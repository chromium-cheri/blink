// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "config.h"
#include "platform/transforms/TransformationMatrix.h"

#include <gtest/gtest.h>

namespace blink {

TEST(TransformationMatrixTest, NonInvertableBlendTest)
{
    TransformationMatrix from;
    TransformationMatrix to(2.7133590938, 0.0, 0.0, 0.0, 0.0, 2.4645137761, 0.0, 0.0, 0.0, 0.0, 0.00, 0.01, 0.02, 0.03, 0.04, 0.05);
    TransformationMatrix result;

    result = to;
    result.blend(from, 0.25);
    EXPECT_TRUE(result == from);

    result = to;
    result.blend(from, 0.75);
    EXPECT_TRUE(result == to);
}

TEST(TransformationMatrixTest, IsIdentityOr2DTranslation)
{
    TransformationMatrix matrix;
    EXPECT_TRUE(matrix.isIdentityOr2DTranslation());

    matrix.makeIdentity();
    matrix.translate(10, 0);
    EXPECT_TRUE(matrix.isIdentityOr2DTranslation());

    matrix.makeIdentity();
    matrix.translate(0, -20);
    EXPECT_TRUE(matrix.isIdentityOr2DTranslation());

    matrix.makeIdentity();
    matrix.translate3d(0, 0, 1);
    EXPECT_FALSE(matrix.isIdentityOr2DTranslation());

    matrix.makeIdentity();
    matrix.rotate(40 /* degrees */);
    EXPECT_FALSE(matrix.isIdentityOr2DTranslation());

    matrix.makeIdentity();
    matrix.skewX(30 /* degrees */);
    EXPECT_FALSE(matrix.isIdentityOr2DTranslation());
}

TEST(TransformationMatrixTest, To2DTranslation)
{
    TransformationMatrix matrix;
    EXPECT_EQ(FloatSize(), matrix.to2DTranslation());
    matrix.translate(30, -40);
    EXPECT_EQ(FloatSize(30, -40), matrix.to2DTranslation());
}

TEST(TransformationMatrixTest, ApplyTransformOrigin)
{
    TransformationMatrix matrix;

    // (0,0,0) is a fixed point of this scale.
    // (1,1,1) should be scaled appropriately.
    matrix.scale3d(2, 3, 4);
    EXPECT_EQ(FloatPoint3D(0, 0, 0), matrix.mapPoint(FloatPoint3D(0, 0, 0)));
    EXPECT_EQ(FloatPoint3D(2, 3, -4), matrix.mapPoint(FloatPoint3D(1, 1, -1)));

    // With the transform origin applied, (1,2,3) is the fixed point.
    // (0,0,0) should be scaled according to its distance from (1,2,3).
    matrix.applyTransformOrigin(1, 2, 3);
    EXPECT_EQ(FloatPoint3D(1, 2, 3), matrix.mapPoint(FloatPoint3D(1, 2, 3)));
    EXPECT_EQ(FloatPoint3D(-1, -4, -9), matrix.mapPoint(FloatPoint3D(0, 0, 0)));
}

} // namespace blink
