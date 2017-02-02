/*
 * Core: Common Code used by other modules of Thanuva
 *
 * Copyright 2016, Saravanan Poosanthiram
 * All rights reserved.
 */

#include "Quaternion.h"

#include <fmt/format.h>
#ifdef UNIT_TEST
#include <gtest/gtest.h>
#endif

namespace Core {

std::string Quaternion::str() const
{
    return fmt::format("({}, {}, {}, {})", m_coords[0], m_coords[1], m_coords[2], m_coords[3]);
}

#ifdef UNIT_TEST

TEST(QuaternionTest, Simple)
{
    Quaternion q{234.56, 26.63, 2.14, 235.244};
    EXPECT_EQ(234.56, q.x());
    EXPECT_EQ(26.63, q.y());
    EXPECT_EQ(2.14, q.z());
    EXPECT_EQ(235.244, q.w());

    Quaternion q2{Vector3d{1.0, 0.0, 0.0}, 90.0};
    Quaternion expected{0.7071067811865475244, 0.0, 0.0, 0.707106781186547524400};
    EXPECT_EQ(expected, q2);
    EXPECT_EQ(1.0, q2.norm());
}

#endif // UNIT_TEST

} // namespace Core
