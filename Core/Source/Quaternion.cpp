/*
 * Core: Common Code used by other modules of Thanuva
 *
 * Copyright 2016, Saravanan Poosanthiram
 * All rights reserved.
 */

#include "Quaternion.h"

#include <AlgoBase.h>
#ifdef UNIT_TEST
#include <gtest/gtest.h>
#endif

#include "Vector3d.h"

namespace Core {

Quaternion::Quaternion(Vector3d v, double angle)
{
    if (!psa::isequal(1.0, v.norm()))
        v.normalize();
    angle /= 2.0;
    angle *= psa::kRadianMultiplier;
    v.scale(std::sin(angle));
    this->initialize(v.x(), v.y(), v.z(), std::cos(angle));
}

bool Quaternion::iszero() const
{
    return psa::iszero(m_quaternion[0]) && psa::iszero(m_quaternion[1])
            && psa::iszero(m_quaternion[2]) && psa::iszero(m_quaternion[3]);
}

bool Quaternion::operator==(const Quaternion& rhs) const
{
    return psa::isequal(rhs.m_quaternion[0], m_quaternion[0])
        && psa::isequal(rhs.m_quaternion[1], m_quaternion[1])
        && psa::isequal(rhs.m_quaternion[2], m_quaternion[2])
        && psa::isequal(rhs.m_quaternion[3], m_quaternion[3]);
}

Quaternion Quaternion::operator*(const Quaternion& rhs) const
{
    Vector3d v{m_quaternion[0], m_quaternion[1], m_quaternion[2]};
    Vector3d v2{rhs.m_quaternion[0], rhs.m_quaternion[1], rhs.m_quaternion[2]};
    Vector3d v3{v.cross(v2) + m_quaternion[3] * v2 + rhs.m_quaternion[3] * v};
    return Quaternion{v3[0], v3[1], v3[2], m_quaternion[3] * rhs.m_quaternion[3] - v.dot(v2)};
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
