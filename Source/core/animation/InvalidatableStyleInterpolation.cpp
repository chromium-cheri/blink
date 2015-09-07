// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "config.h"
#include "core/animation/InvalidatableStyleInterpolation.h"

#include "core/animation/StringKeyframe.h"
#include "core/css/resolver/StyleResolverState.h"

namespace blink {

InvalidatableStyleInterpolation::InvalidatableStyleInterpolation(
    const Vector<const InterpolationType*>& interpolationTypes,
    const CSSPropertySpecificKeyframe& startKeyframe,
    const CSSPropertySpecificKeyframe& endKeyframe)
    : StyleInterpolation(nullptr, nullptr, interpolationTypes.first()->property())
    , m_interpolationTypes(interpolationTypes)
    , m_startKeyframe(&startKeyframe)
    , m_endKeyframe(&endKeyframe)
{
    maybeCachePairwiseConversion(nullptr, nullptr);
    interpolate(0, 0);
}

bool InvalidatableStyleInterpolation::maybeCachePairwiseConversion(const StyleResolverState* state, const InterpolationValue* underlyingValue) const
{
    for (const auto& interpolationType : m_interpolationTypes) {
        if ((m_startKeyframe->isNeutral() || m_endKeyframe->isNeutral()) && (!underlyingValue || underlyingValue->type() != *interpolationType))
            continue;
        OwnPtr<PairwisePrimitiveInterpolation> pairwiseConversion = interpolationType->maybeConvertPairwise(*m_startKeyframe, *m_endKeyframe, state, m_conversionCheckers);
        if (pairwiseConversion) {
            m_cachedValue = pairwiseConversion->initialValue();
            m_cachedConversion = pairwiseConversion.release();
            return true;
        }
    }
    return false;
}

void InvalidatableStyleInterpolation::interpolate(int, double fraction)
{
    m_currentFraction = fraction;
    if (m_cachedConversion)
        m_cachedConversion->interpolateValue(fraction, m_cachedValue);
    // We defer the interpolation to ensureValidInterpolation() if m_cachedConversion is null.
}

PassOwnPtr<InterpolationValue> InvalidatableStyleInterpolation::convertSingleKeyframe(const CSSPropertySpecificKeyframe& keyframe, const StyleResolverState& state, const InterpolationValue* underlyingValue) const
{
    if (keyframe.isNeutral() && !underlyingValue)
        return nullptr;
    for (const auto& interpolationType : m_interpolationTypes) {
        if (keyframe.isNeutral() && underlyingValue->type() != *interpolationType)
            continue;
        OwnPtr<InterpolationValue> result = interpolationType->maybeConvertSingle(keyframe, &state, m_conversionCheckers);
        if (result)
            return result.release();
    }
    ASSERT(keyframe.isNeutral());
    return nullptr;
}

PassOwnPtr<InterpolationValue> InvalidatableStyleInterpolation::maybeConvertUnderlyingValue(const StyleResolverState& state) const
{
    for (const auto& interpolationType : m_interpolationTypes) {
        OwnPtr<InterpolationValue> result = interpolationType->maybeConvertUnderlyingValue(state);
        if (result)
            return result.release();
    }
    return nullptr;
}

bool InvalidatableStyleInterpolation::dependsOnUnderlyingValue() const
{
    return (m_startKeyframe->underlyingFraction() != 0 && m_currentFraction != 1) || (m_endKeyframe->underlyingFraction() != 0 && m_currentFraction != 0);
}

bool InvalidatableStyleInterpolation::isNeutralKeyframeActive() const
{
    return (m_startKeyframe->isNeutral() && m_currentFraction != 1) || (m_endKeyframe->isNeutral() && m_currentFraction != 0);
}

bool InvalidatableStyleInterpolation::isCacheValid(const StyleResolverState& state, const InterpolationValue* underlyingValue) const
{
    if (isNeutralKeyframeActive()) {
        if (m_cachedConversion->isFlip())
            return false;
        // Pairwise interpolation can never happen between different InterpolationTypes, neutral values always represent the underlying value.
        if (!underlyingValue || !m_cachedValue || m_cachedValue->type() != underlyingValue->type())
            return false;
    }
    for (const auto& checker : m_conversionCheckers) {
        if (!checker->isValid(state))
            return false;
    }
    return true;
}

const InterpolationValue* InvalidatableStyleInterpolation::ensureValidInterpolation(const StyleResolverState& state, const InterpolationValue* underlyingValue) const
{
    if (m_cachedConversion && isCacheValid(state, underlyingValue))
        return m_cachedValue.get();
    m_conversionCheckers.clear();
    if (!maybeCachePairwiseConversion(&state, underlyingValue)) {
        m_cachedConversion = FlipPrimitiveInterpolation::create(
            convertSingleKeyframe(*m_startKeyframe, state, underlyingValue),
            convertSingleKeyframe(*m_endKeyframe, state, underlyingValue));
    }
    m_cachedConversion->interpolateValue(m_currentFraction, m_cachedValue);
    return m_cachedValue.get();
}

void InvalidatableStyleInterpolation::setFlagIfInheritUsed(StyleResolverState& state) const
{
    if (!state.parentStyle())
        return;
    if ((m_startKeyframe->value() && m_startKeyframe->value()->isInheritedValue())
        || (m_endKeyframe->value() && m_endKeyframe->value()->isInheritedValue())) {
        state.parentStyle()->setHasExplicitlyInheritedProperties();
    }
}

// Handles memory management of underlying InterpolationValues in applyStack()
// Ensures we perform copy on write if we are not the owner of an underlying InterpolationValue.
// This functions similar to a DataRef except on OwnPtr'd objects.
class UnderlyingValue {
    STACK_ALLOCATED();
public:
    UnderlyingValue()
        : m_owner()
        , m_pointer(nullptr)
    { }

    void set(const InterpolationValue* interpolationValue)
    {
        // By clearing m_owner we will perform a copy when attempting to access()
        // m_pointer as a mutable reference, thus upholding the const contract for
        // this instance of interpolationValue despite the const_cast.
        m_owner.clear();
        m_pointer = const_cast<InterpolationValue*>(interpolationValue);
    }
    void set(PassOwnPtr<InterpolationValue> interpolationValue)
    {
        m_owner = interpolationValue;
        m_pointer = m_owner.get();
    }
    InterpolationValue& access()
    {
        ASSERT(m_pointer);
        if (!m_owner)
            set(m_pointer->clone());
        return *m_pointer;
    }
    const InterpolationValue* get() const { return m_pointer; }
    operator bool() const { return m_pointer; }
    const InterpolationValue* operator->() const
    {
        ASSERT(m_pointer);
        return m_pointer;
    }

private:
    OwnPtr<InterpolationValue> m_owner;
    InterpolationValue* m_pointer;
};

void InvalidatableStyleInterpolation::applyStack(const ActiveInterpolations& interpolations, StyleResolverState& state)
{
    ASSERT(!interpolations.isEmpty());
    size_t startingIndex = 0;

    // Compute the underlying value to composite onto.
    UnderlyingValue underlyingValue;
    const InvalidatableStyleInterpolation& firstInterpolation = toInvalidatableStyleInterpolation(*interpolations.at(startingIndex));
    if (firstInterpolation.dependsOnUnderlyingValue()) {
        underlyingValue.set(firstInterpolation.maybeConvertUnderlyingValue(state));
    } else {
        const InterpolationValue* firstValue = firstInterpolation.ensureValidInterpolation(state, nullptr);
        // Fast path for replace interpolations that are the only one to apply.
        if (interpolations.size() == 1) {
            if (firstValue) {
                firstInterpolation.setFlagIfInheritUsed(state);
                firstValue->type().apply(firstValue->interpolableValue(), firstValue->nonInterpolableValue(), state);
            }
            return;
        }
        underlyingValue.set(firstValue);
        startingIndex++;
    }

    // Composite interpolations onto the underlying value.
    bool shouldApply = false;
    for (size_t i = startingIndex; i < interpolations.size(); i++) {
        const InvalidatableStyleInterpolation& currentInterpolation = toInvalidatableStyleInterpolation(*interpolations.at(i));
        ASSERT(currentInterpolation.dependsOnUnderlyingValue());
        const InterpolationValue* currentValue = currentInterpolation.ensureValidInterpolation(state, underlyingValue.get());
        if (!currentValue)
            continue;
        shouldApply = true;
        currentInterpolation.setFlagIfInheritUsed(state);
        if (!underlyingValue || underlyingValue->type() != currentValue->type()) {
            underlyingValue.set(currentValue);
        } else {
            double underlyingFraction = currentInterpolation.m_cachedConversion->interpolateUnderlyingFraction(
                currentInterpolation.m_startKeyframe->underlyingFraction(),
                currentInterpolation.m_endKeyframe->underlyingFraction(),
                currentInterpolation.m_currentFraction);
            underlyingValue.access().interpolableValue().scaleAndAdd(underlyingFraction, currentInterpolation.m_cachedValue->interpolableValue());
        }
    }

    if (shouldApply && underlyingValue)
        underlyingValue->type().apply(underlyingValue->interpolableValue(), underlyingValue->nonInterpolableValue(), state);
}

} // namespace blink
