/*
 * Core: Common Code used by other modules of Thanuva
 *
 * Copyright 2016, Saravanan Poosanthiram
 * All rights reserved.
 */

#ifndef CORE_QUATERNION_H
#define CORE_QUATERNION_H

#include <array>
#include <cmath>

#include <AlgoBase.h>

#include "Vector3d.h"

namespace Core {

class Quaternion
{
public:
    Quaternion() { this->initialize(0.0, 0.0, 0.0, 1.0); }
    Quaternion(double x, double y, double z, double w) { this->initialize(x, y, z, w); }
    Quaternion(Vector3d v, double angle)
    {
        if (!psa::isequal(1.0, v.norm()))
            v.normalize();
        angle /= 2.0;
        angle *= psa::kRadianMultiplier;
        v.scale(std::sin(angle));
        this->initialize(v.x(), v.y(), v.z(), std::cos(angle));
    }

    double x() const { return m_coords[0]; }
    double y() const { return m_coords[1]; }
    double z() const { return m_coords[2]; }
    double w() const { return m_coords[3]; }
    std::string str() const;

    void setX(double x) { m_coords[0] = x; }
    void setY(double y) { m_coords[1] = y; }
    void setZ(double z) { m_coords[2] = z; }
    void setW(double w) { m_coords[3] = w; }
    void assign(double x, double y, double z, double w) { this->initialize(x, y, z, w); }

    bool iszero() const
    {
        return psa::iszero(m_coords[0]) && psa::iszero(m_coords[1]) &&
            psa::iszero(m_coords[2]) && psa::iszero(m_coords[3]);
    }

    double norm() const
    {
        return std::sqrt(m_coords[0] * m_coords[0] + m_coords[1] * m_coords[1]
                         + m_coords[2] * m_coords[2] + m_coords[3] * m_coords[3]);
    }

    void normalize()
    {
        double norm = this->norm();
        this->initialize(m_coords[0] / norm, m_coords[1] / norm,
                         m_coords[2] / norm, m_coords[3] / norm);
    }

    bool operator==(const Quaternion& rhs) const
    {
        return psa::isequal(rhs.m_coords[0], m_coords[0]) &&
            psa::isequal(rhs.m_coords[1], m_coords[1]) &&
            psa::isequal(rhs.m_coords[2], m_coords[2]) &&
            psa::isequal(rhs.m_coords[3], m_coords[3]);
    }
    bool operator!=(const Quaternion& rhs) const { return !(rhs == *this); }

    Quaternion& operator+=(const Quaternion& rhs)
    {
        m_coords[0] += rhs.m_coords[0];
        m_coords[1] += rhs.m_coords[1];
        m_coords[2] += rhs.m_coords[2];
        m_coords[3] += rhs.m_coords[3];

        return *this;
    }
    Quaternion operator+(const Quaternion& rhs) const
    {
        return Quaternion{m_coords[0] + rhs.m_coords[0],
            m_coords[1] + rhs.m_coords[1],
            m_coords[2] + rhs.m_coords[2],
            m_coords[3] + rhs.m_coords[3]};
    }

    Quaternion& operator*=(const Quaternion& rhs)
    {
        *this = *this * rhs;
        return *this;
    }
    Quaternion operator*(const Quaternion& rhs) const
    {
        Vector3d v{m_coords[0], m_coords[1], m_coords[2]};
        Vector3d v2{rhs.m_coords[0], rhs.m_coords[1], rhs.m_coords[2]};
        Vector3d v3{v.cross(v2) + m_coords[3] * v2 + rhs.m_coords[3] * v};
        return Quaternion{v3[0], v3[1], v3[2], m_coords[3] * rhs.m_coords[3] - v.dot(v2)};
    }

private:
    void initialize(double x, double y, double z, double w)
    {
        m_coords[0] = x;
        m_coords[1] = y;
        m_coords[2] = z;
        m_coords[3] = w;
    }

    std::array<double, 4> m_coords;
};

} // namespace Core

#endif // CORE_QUATERNION_H
