// Use of this source code is governed by a BSD-style license that can be
// Copyright 2014 The Chromium Authors. All rights reserved.
// found in the LICENSE file.

#include "config.h"
#include "core/css/MediaValues.h"

#include "core/css/CSSPrimitiveValue.h"
#include "wtf/text/StringBuilder.h"

#include <gtest/gtest.h>

namespace blink {

struct TestCase {
    double value;
    CSSPrimitiveValue::UnitType type;
    unsigned fontSize;
    unsigned viewportWidth;
    unsigned viewportHeight;
    bool success;
    int output;
};

TEST(MediaValuesTest, Basic)
{
    TestCase testCases[] = {
        { 40.0, CSSPrimitiveValue::UnitType::Pixels, 16, 300, 300, true, 40 },
        { 40.0, CSSPrimitiveValue::UnitType::Ems, 16, 300, 300, true, 640 },
        { 40.0, CSSPrimitiveValue::UnitType::Rems, 16, 300, 300, true, 640 },
        { 40.0, CSSPrimitiveValue::UnitType::Exs, 16, 300, 300, true, 320 },
        { 40.0, CSSPrimitiveValue::UnitType::Chs, 16, 300, 300, true, 320 },
        { 43.0, CSSPrimitiveValue::UnitType::ViewportWidth, 16, 848, 976, true, 364 },
        { 43.0, CSSPrimitiveValue::UnitType::ViewportHeight, 16, 848, 976, true, 419 },
        { 43.0, CSSPrimitiveValue::UnitType::ViewportMin, 16, 848, 976, true, 364 },
        { 43.0, CSSPrimitiveValue::UnitType::ViewportMax, 16, 848, 976, true, 419 },
        { 1.3, CSSPrimitiveValue::UnitType::Centimeters, 16, 300, 300, true, 49 },
        { 1.3, CSSPrimitiveValue::UnitType::Millimeters, 16, 300, 300, true, 4 },
        { 1.3, CSSPrimitiveValue::UnitType::Inches, 16, 300, 300, true, 124 },
        { 13, CSSPrimitiveValue::UnitType::Points, 16, 300, 300, true, 17 },
        { 1.3, CSSPrimitiveValue::UnitType::Picas, 16, 300, 300, true, 20 },
        { 1.3, CSSPrimitiveValue::UnitType::Unknown, 16, 300, 300, false, 20 },
        { 0.0, CSSPrimitiveValue::UnitType::Unknown, 0, 0, 0, false, 0.0 } // Do not remove the terminating line.
    };


    for (unsigned i = 0; testCases[i].viewportWidth; ++i) {
        int output = 0;
        bool success = MediaValues::computeLength(testCases[i].value,
            testCases[i].type,
            testCases[i].fontSize,
            testCases[i].viewportWidth,
            testCases[i].viewportHeight,
            output);
        ASSERT_EQ(testCases[i].success, success);
        if (success)
            ASSERT_EQ(testCases[i].output, output);
    }
}

} // namespace
