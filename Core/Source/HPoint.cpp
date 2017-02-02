/*
 * Core: Common Code used by other modules of Thanuva
 *
 * Copyright 2016, Saravanan Poosanthiram
 * All rights reserved.
 */

#include "HPoint.h"

#include <fmt/format.h>
#ifdef UNIT_TEST
#include <gtest/gtest.h>
#endif

namespace Core {

std::string HPoint::str() const
{
    return fmt::format("({}, {}, {}, {})", m_coords[0], m_coords[1], m_coords[2], m_coords[3]);
}

#ifdef UNIT_TEST

TEST(HPointTest, Creation)
{
    HPoint hv{};
    EXPECT_EQ(0.0, hv.x());
    EXPECT_EQ(0.0, hv.y());
    EXPECT_EQ(0.0, hv.z());
    EXPECT_EQ(0.0, hv.w());

    HPoint hv2{0.34, 90, 34.6e-4, 2.342e+56};
    EXPECT_EQ(0.34, hv2.x());
    EXPECT_EQ(90.0, hv2[1]);
    EXPECT_EQ(3.46e-3, hv2[2]);
    EXPECT_EQ(2.342e+56, hv2.w());

    double values[] = {1.0, 0.0, 0.0, 1.0};
    HPoint hv3{values};
    EXPECT_EQ(1.0, hv3[0]);
    EXPECT_EQ(0.0, hv3.data()[1]);
    EXPECT_EQ(0.0, hv3.z());
    EXPECT_EQ(1.0, hv3[3]);
}

TEST(HPointTest, AssignAndEquality)
{
    HPoint hv{};
    HPoint hv2{0.34, 90, 34.6e-4, 2.342e+56};
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

#endif // UNIT_TEST

} //namespace Core
