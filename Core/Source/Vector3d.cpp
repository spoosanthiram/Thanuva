/*
 * Core: Common Code used by other modules of Thanuva
 *
 * Copyright 2016, Saravanan Poosanthiram
 * All rights reserved.
 */

#include "Vector3d.h"

#include <sstream>

#ifdef UNIT_TEST
#include <gtest/gtest.h>
#endif

#include "HVector.h"

namespace Core {

Vector3d::Vector3d(const HVector& hv)
{
    this->initialize(hv.x(), hv.y(), hv.z());
}

std::string Vector3d::str() const
{
    std::stringstream sstream;
    sstream << "(" << m_elements[0] << ", " << m_elements[1] << ", " << m_elements[2] << ")";
    return sstream.str();
}

void Vector3d::assign(const HVector& hv)
{
    this->initialize(hv.x(), hv.y(), hv.z());
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

#endif // UNIT_TEST

} // namespace Core
