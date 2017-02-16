/*
 * Core: Common Code used by other modules of Thanuva
 *
 * Copyright 2016, Saravanan Poosanthiram
 * All rights reserved.
 */

#include "Vector3d.h"

#include <cassert>
#include <cmath>

#include <fmt/format.h>
#ifdef UNIT_TEST
#include <gtest/gtest.h>
#endif

namespace Core {

std::string Vector3d::str() const
{
    return fmt::format("({}, {}, {})", m_coords[0], m_coords[1], m_coords[2]);
}

Vector3d Vector3d::orthonormal() const
{
    assert(psa::isequal(this->norm(), 1.0));

    // construct vector{t} by replacing minimum absolute value by 1.0
    Vector3d t{m_coords.data()};
    int minIndex = 0;
    for (auto i = 1; i < m_coords.size(); ++i) {
        if (std::abs(t[i]) <= std::abs(t[minIndex]))
            minIndex = i;
    }
    t.m_coords[minIndex] = 1.0;

    // (t x this) is one of infinitly many orthogonal vector
    Vector3d u = t.cross(*this);
    u.normalize();

    return u;
}

#ifdef UNIT_TEST

TEST(Vector3dTest, Creation)
{
    Vector3d v;
    EXPECT_EQ(0.0, v.x());
    EXPECT_EQ(0.0, v.y());
    EXPECT_EQ(0.0, v.z());

    double x = 0.033, y = 0.6331, z = 53.54641;
    v = Vector3d{x, y, z};
    EXPECT_EQ(x, v.x());
    EXPECT_EQ(y, v.y());
    EXPECT_EQ(z, v.z());

    std::vector<float> values{87.09f, 8.5636e-9f, 12.53e+12f};
    v = Vector3d{values.data()};
    EXPECT_EQ(values[0], v.x());
    EXPECT_EQ(values[1], v.y());
    EXPECT_EQ(values[2], v.z());
}

TEST(Vector3dTest, DotProduct)
{
    Vector3d basisX{ 1.0, 0.0, 0.0 };
    Vector3d basisY{ 0.0, 1.0, 0.0 };
    double actual = basisX.dot(basisY);
    EXPECT_EQ(0.0, actual);
}

TEST(Vector3dTest, PerpendicularVector)
{
    Vector3d w{1.0, 0.0, 0.0};
    Vector3d u = w.orthonormal();
    Vector3d expected{0.0, 1.0, 0.0};
    EXPECT_EQ(expected, u);

    w = Vector3d{0.0, 0.0, 1.0};
    u = w.orthonormal();
    expected = Vector3d{1.0, 0.0, 0.0};
    EXPECT_EQ(expected, u);

    w = Vector3d{0.0, 0.0, 1.0} - Vector3d{1.0, 1.0, 0.0};
    w.normalize();
    u = w.orthonormal();
    expected = Vector3d{0.70710678118654757, -0.70710678118654757, 0.0};
    EXPECT_EQ(expected, u);
}

#endif // UNIT_TEST

} // namespace Core
