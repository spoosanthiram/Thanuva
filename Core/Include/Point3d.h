/*
 * Core: Common Code used by other modules of Thanuva
 *
 * Copyright 2016, Saravanan Poosanthiram
 * All rights reserved.
 */

#ifndef CORE_POINT3D_H
#define CORE_POINT3D_H

#include <array>
#include <string>

#include <AlgoBase.h>

#include "Vector3d.h"

namespace Core {

class Point3d
{
public:
    Point3d() { this->initialize(0.0, 0.0, 0.0); }
    Point3d(float x, float y, float z) { this->initialize(x, y, z); }
    Point3d(double x, double y, double z) { this->initialize(x, y, z); }
    explicit Point3d(const float* values) { this->initialize(values); }
    explicit Point3d(const double* values) { this->initialize(values); }

    double x() const { return m_coords[0]; }
    double y() const { return m_coords[1]; }
    double z() const { return m_coords[2]; }
    double operator[] (std::size_t i) const { return m_coords[i]; }
    std::size_t size() const { return m_coords.size(); }
    const double* data() const { return m_coords.data(); }
    std::string str() const;

    void setX(double x) { m_coords[0] = x; }
    void setY(double y) { m_coords[1] = y; }
    void setZ(double z) { m_coords[2] = z; }
    void assign(float x, float y, float z) { this->initialize(x, y, z); }
    void assign(double x, double y, double z) { this->initialize(x, y, z); }
    void assign(const float* values) { this->initialize(values); }
    void assign(const double* values) { this->initialize(values); }

    bool iszero() const
    {
        return psa::iszero(m_coords[0]) &&
            psa::iszero(m_coords[1]) &&
            psa::iszero(m_coords[2]);
    }

    bool operator==(const Point3d& rhs) const
    {
        return psa::isequal(rhs.m_coords[0], m_coords[0])
                && psa::isequal(rhs.m_coords[1], m_coords[1])
                && psa::isequal(rhs.m_coords[2], m_coords[2]);
    }
    bool operator!=(const Point3d& rhs) const { return !(rhs == *this); }

    Point3d& operator+=(const Vector3d& v)
    {
        m_coords[0] += v[0];
        m_coords[1] += v[1];
        m_coords[2] += v[2];

        return *this;
    }
    Point3d operator+(const Vector3d& v) const
    {
        return Point3d{m_coords[0] + v[0], m_coords[1] + v[1], m_coords[2] + v[2]};
    }

    Point3d& operator-=(const Vector3d& v)
    {
        m_coords[0] -= v[0];
        m_coords[1] -= v[1];
        m_coords[2] -= v[2];

        return *this;
    }
    Point3d operator-(const Vector3d& v) const
    {
        return Point3d{m_coords[0] - v[0], m_coords[1] - v[1], m_coords[2] - v[2]};
    }

    Vector3d operator-(const Point3d& rhs) const
    {
        return Vector3d{m_coords[0] - rhs.m_coords[0],
            m_coords[1] - rhs.m_coords[1],
            m_coords[2] - rhs.m_coords[2]};
    }

private:
    void initialize(double x, double y, double z)
    {
        m_coords[0] = x;
        m_coords[1] = y;
        m_coords[2] = z;
    }
    void initialize(const float* values)
    {
        this->initialize(values[0], values[1], values[2]);
    }
    void initialize(const double* values)
    {
        this->initialize(values[0], values[1], values[2]);
    }

    std::array<double, 3> m_coords;
};

} // namespace Core

#endif // CORE_POINT3D_H
