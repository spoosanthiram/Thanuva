/*
 * Core: Common Code used by other modules of Thanuva
 *
 * Copyright 2016, Saravanan Poosanthiram
 * All rights reserved.
 */

#include "HVector.h"

#ifdef UNIT_TEST
#include <gtest/gtest.h>
#endif

#include "Vector3d.h"

namespace Core {

HVector::HVector(const Vector3d& v)
{
    this->initialize(v.x(), v.y(), v.z(), 1.0);
}

void HVector::assign(const Vector3d& v)
{
    this->initialize(v.x(), v.y(), v.z(), 1.0);
}

#ifdef UNIT_TEST

TEST(HVectorTest, Creation)
{
    HVector hv{};
    EXPECT_EQ(0.0, hv.x());
    EXPECT_EQ(0.0, hv.y());
    EXPECT_EQ(0.0, hv.z());
    EXPECT_EQ(0.0, hv.w());

    HVector hv2{0.34, 90, 34.6e-4, 2.342e+56};
    EXPECT_EQ(0.34, hv2.x());
    EXPECT_EQ(90.0, hv2[1]);
    EXPECT_EQ(3.46e-3, hv2[2]);
    EXPECT_EQ(2.342e+56, hv2.w());

    double values[] = {1.0, 0.0, 0.0, 1.0};
    HVector hv3{values};
    EXPECT_EQ(1.0, hv3[0]);
    EXPECT_EQ(0.0, hv3.data()[1]);
    EXPECT_EQ(0.0, hv3.z());
    EXPECT_EQ(1.0, hv3[3]);
}

TEST(HVectorTest, AssignAndEquality)
{
    HVector hv{};
    HVector hv2{0.34, 90, 34.6e-4, 2.342e+56};
    hv = hv2;
    EXPECT_EQ(0.34, hv2.x());
    EXPECT_EQ(2.342e+56, hv2.w());

    double vv[] = {1.0, 0.97789, 3.45745e+67, -345345.0};
    hv.assign(vv);
    hv2.assign(vv[0], vv[1], vv[2], vv[3]);
    EXPECT_TRUE(hv == hv2);
    EXPECT_FALSE(hv != hv2);
    hv.setX(98345340.9893434);
    EXPECT_TRUE(hv != hv2);
    EXPECT_FALSE(hv == hv2);
}

TEST(HVectorTest, SimpleOperations)
{
    HVector hv{9.9, 0.0, -3.64, 0.0};
    HVector hv2{2.0, 4.8, 343.64, -9989.9};
    HVector expected{11.9, 4.8, 340.0, -9989.9};
    EXPECT_EQ(expected, hv + hv2);
}

#endif // UNIT_TEST

} //namespace Core
