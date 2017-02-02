/*
 * Core: Common Code used by other modules of Thanuva
 *
 * Copyright 2016, Saravanan Poosanthiram
 * All rights reserved.
 */

#include "Point3d.h"

#include <fmt/format.h>
#ifdef UNIT_TEST
#include <gtest/gtest.h>
#endif

namespace Core {

std::string Point3d::str() const
{
    return fmt::format("({}, {}, {})", m_coords[0], m_coords[1], m_coords[2]);
}

#ifdef UNIT_TEST

TEST(Point3dTest, Creation)
{
    Point3d p;
    EXPECT_EQ(0.0, p.x());
    EXPECT_EQ(0.0, p.y());
    EXPECT_EQ(0.0, p.z());

    double x = 0.033, y = 0.6331, z = 53.54641;
    p = Point3d{x, y, z};
    EXPECT_EQ(x, p.x());
    EXPECT_EQ(y, p.y());
    EXPECT_EQ(z, p.z());

    std::vector<float> values{87.09f, 8.5636e-9f, 12.53e+12f};
    p = Point3d{values.data()};
    EXPECT_EQ(values[0], p.x());
    EXPECT_EQ(values[1], p.y());
    EXPECT_EQ(values[2], p.z());
}

#endif // UNIT_TEST

} // namespace Core
