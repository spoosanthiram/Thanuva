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

namespace Core {

class Vector3d;

class Quaternion
{
public:
    Quaternion() { this->initialize(0.0, 0.0, 0.0, 1.0); }
    Quaternion(double x, double y, double z, double w) { this->initialize(x, y, z, w); }
    Quaternion(Vector3d v, double angle);

    double x() const { return m_quaternion[0]; }
    double y() const { return m_quaternion[1]; }
    double z() const { return m_quaternion[2]; }
    double w() const { return m_quaternion[3]; }
    double norm() const
    {
        return std::sqrt(m_quaternion[0] * m_quaternion[0] + m_quaternion[1] * m_quaternion[1]
                + m_quaternion[2] * m_quaternion[2] + m_quaternion[3] * m_quaternion[3]);
    }
    bool iszero() const;

    void setX(double x) { m_quaternion[0] = x; }
    void setY(double y) { m_quaternion[1] = y; }
    void setZ(double z) { m_quaternion[2] = z; }
    void setW(double w) { m_quaternion[3] = w; }
    void assign(double x, double y, double z, double w) { this->initialize(x, y, z, w); }

    void normalize()
    {
        double norm = this->norm();
        this->initialize(m_quaternion[0] / norm, m_quaternion[1] / norm,
                         m_quaternion[2] / norm, m_quaternion[3] / norm);
    }

    bool operator==(const Quaternion& rhs) const;
    bool operator!=(const Quaternion& rhs) const { return !(rhs == *this); }

    Quaternion& operator+=(const Quaternion& rhs)
    {
        m_quaternion[0] += rhs.m_quaternion[0];
        m_quaternion[1] += rhs.m_quaternion[1];
        m_quaternion[2] += rhs.m_quaternion[2];
        m_quaternion[3] += rhs.m_quaternion[3];
        return *this;
    }
    Quaternion operator+(const Quaternion& rhs) const
    {
        Quaternion q{*this};
        q += rhs;
        return q;
    }

    Quaternion& operator*=(const Quaternion& rhs) { *this = *this * rhs; return *this; }
    Quaternion operator*(const Quaternion& rhs) const;

private:
    void initialize(double x, double y, double z, double w)
    {
        m_quaternion[0] = x;
        m_quaternion[1] = y;
        m_quaternion[2] = z;
        m_quaternion[3] = w;
    }

    std::array<double, 4> m_quaternion;
};

} // namespace Core

#endif // CORE_QUATERNION_H
