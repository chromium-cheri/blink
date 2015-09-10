// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef DisplayItemListPaintTest_h
#define DisplayItemListPaintTest_h

#include "core/frame/FrameView.h"
#include "core/layout/LayoutTestHelper.h"
#include "core/layout/LayoutView.h"
#include "core/paint/DeprecatedPaintLayer.h"
#include "platform/graphics/GraphicsLayer.h"
#include <gtest/gtest.h>

namespace blink {

class DisplayItemListPaintTest : public RenderingTest {
public:
    DisplayItemListPaintTest()
        : m_originalSlimmingPaintEnabled(RuntimeEnabledFeatures::slimmingPaintEnabled()) { }

protected:
    LayoutView& layoutView() { return *document().layoutView(); }
    DisplayItemList& rootDisplayItemList() { return *layoutView().layer()->graphicsLayerBacking()->displayItemList(); }

private:
    void SetUp() override
    {
        RuntimeEnabledFeatures::setSlimmingPaintEnabled(true);

        RenderingTest::SetUp();
        enableCompositing();
    }

    void TearDown() override
    {
        RuntimeEnabledFeatures::setSlimmingPaintEnabled(m_originalSlimmingPaintEnabled);
    }

    bool m_originalSlimmingPaintEnabled;
};

// Slimming paint v2 has subtly different behavior on some paint tests. This
// class is used to test only v2 behavior while maintaining v1 test coverage.
class DisplayItemListPaintTestForSlimmingPaintV2 : public RenderingTest {
public:
    DisplayItemListPaintTestForSlimmingPaintV2()
        : m_originalSlimmingPaintV2Enabled(RuntimeEnabledFeatures::slimmingPaintV2Enabled()) { }

protected:
    LayoutView& layoutView() { return *document().layoutView(); }
    DisplayItemList& rootDisplayItemList() { return *layoutView().layer()->graphicsLayerBacking()->displayItemList(); }

    // Expose some document lifecycle steps for checking new display items before commiting.
    void updateLifecyclePhasesToPaintForSlimmingPaintV2Clean(const LayoutRect& interestRect = LayoutRect::infiniteRect())
    {
        document().view()->updateLifecyclePhasesInternal(FrameView::OnlyUpToCompositingCleanPlusScrolling);
        document().view()->invalidateTreeIfNeededRecursive();
        document().view()->paintForSlimmingPaintV2(interestRect);
    }
    void compositeForSlimmingPaintV2() { document().view()->compositeForSlimmingPaintV2(); }

private:
    void SetUp() override
    {
        ASSERT_TRUE(RuntimeEnabledFeatures::slimmingPaintEnabled());
        RuntimeEnabledFeatures::setSlimmingPaintV2Enabled(true);

        RenderingTest::SetUp();
        enableCompositing();
    }

    void TearDown() override
    {
        RuntimeEnabledFeatures::setSlimmingPaintV2Enabled(m_originalSlimmingPaintV2Enabled);
    }

    bool m_originalSlimmingPaintV2Enabled;
};

class TestDisplayItem final : public DisplayItem {
public:
    TestDisplayItem(const DisplayItemClientWrapper& client, Type type) : DisplayItem(client, type, sizeof(*this)) { }

    void replay(GraphicsContext&) final { ASSERT_NOT_REACHED(); }
    void appendToWebDisplayItemList(WebDisplayItemList*) const final { ASSERT_NOT_REACHED(); }
};

#ifndef NDEBUG
#define TRACE_DISPLAY_ITEMS(i, expected, actual) \
    String trace = String::format("%d: ", (int)i) + "Expected: " + (expected).asDebugString() + " Actual: " + (actual).asDebugString(); \
    SCOPED_TRACE(trace.utf8().data());
#else
#define TRACE_DISPLAY_ITEMS(i, expected, actual)
#endif

#define EXPECT_DISPLAY_LIST_BASE(actual, expectedSize, ...) \
    do { \
        EXPECT_EQ((size_t)expectedSize, actual.size()); \
        if (expectedSize != actual.size()) \
            break; \
        const TestDisplayItem expected[] = { __VA_ARGS__ }; \
        for (size_t index = 0; index < std::min<size_t>(actual.size(), expectedSize); index++) { \
            TRACE_DISPLAY_ITEMS(index, expected[index], actual[index]); \
            EXPECT_EQ(expected[index].client(), actual[index].client()); \
            EXPECT_EQ(expected[index].type(), actual[index].type()); \
        } \
    } while (false);

#ifndef NDEBUG
#define EXPECT_DISPLAY_LIST_WITH_RED_FILL_IN_DEBUG(actual, expectedSizeWithoutFill, ...) \
    EXPECT_DISPLAY_LIST_BASE( \
        actual, expectedSizeWithoutFill + 1, \
        TestDisplayItem(*layoutView().layer()->graphicsLayerBacking(), DisplayItem::DebugRedFill), \
        __VA_ARGS__)
#define EXPECT_DISPLAY_LIST_WITH_CACHED_RED_FILL_IN_DEBUG(actual, expectedSizeWithoutFill, ...) \
    EXPECT_DISPLAY_LIST_BASE( \
        actual, expectedSizeWithoutFill + 1, \
        TestDisplayItem(*layoutView().layer()->graphicsLayerBacking(), DisplayItem::drawingTypeToCachedDrawingType(DisplayItem::DebugRedFill)), \
        __VA_ARGS__)
#else
#define EXPECT_DISPLAY_LIST_WITH_RED_FILL_IN_DEBUG EXPECT_DISPLAY_LIST_BASE
#define EXPECT_DISPLAY_LIST_WITH_CACHED_RED_FILL_IN_DEBUG EXPECT_DISPLAY_LIST_BASE
#endif

} // namespace blink

#endif // DisplayItemListPaintTest_h
