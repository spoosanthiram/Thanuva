/*
 * Core: Common Code used by other modules of Thanuva
 *
 * Copyright 2016, Saravanan Poosanthiram
 * All rights reserved.
 */

#ifndef CORE_HVECTOR_H
#define CORE_HVECTOR_H

#include <array>
#include <string>

#include <AlgoBase.h>

#include "Vector3d.h"

namespace Core {

/**
* The HVector class is an abstraction of vector in homogeneous coordinate system.
* see http://en.wikipedia.org/wiki/Homogeneous_coordinates
*/
class HVector
{
public:
    HVector() { this->initialize(0.0, 0.0, 0.0, 0.0); }
    HVector(double x, double y, double z, double w) { this->initialize(x, y, z, w); }
    explicit HVector(const double* values) { this->initialize(values); }
    explicit HVector(const Vector3d& v, double w = 1.0) { this->initialize(v.x(), v.y(), v.z(), w); }

    double x() const { return m_coords[0]; }
    double y() const { return m_coords[1]; }
    double z() const { return m_coords[2]; }
    double w() const { return m_coords[3]; }
    double operator[] (std::size_t i) const { return m_coords[i]; }
    std::size_t size() const { return m_coords.size(); }
    const double* data() const { return m_coords.data(); }
    std::string str() const;

    void setX(double x) { m_coords[0] = x; }
    void setY(double y) { m_coords[1] = y; }
    void setZ(double z) { m_coords[2] = z; }
    void setW(double w) { m_coords[3] = w; }
    void assign(double x, double y, double z, double w) { this->initialize(x, y, z, w); }
    void assign(const double* values)
    {
        this->initialize(values[0], values[1], values[2], values[3]);
    }
    void assign(const Vector3d& v)
    {
        this->initialize(v.x(), v.y(), v.z(), 1.0);
    }

    void scale(double scalar)
    {
        m_coords[0] *= scalar;
        m_coords[1] *= scalar;
        m_coords[2] *= scalar;
        m_coords[3] *= scalar;
    }
    void scaleInfinite(double scalar)
    {
        m_coords[0] = psa::multiply(scalar, m_coords[0]);
        m_coords[1] = psa::multiply(scalar, m_coords[1]);
        m_coords[2] = psa::multiply(scalar, m_coords[2]);
        m_coords[3] = psa::multiply(scalar, m_coords[3]);
    }
    void addInfinite(const HVector& rhs)
    {
        m_coords[0] = psa::add(m_coords[0], rhs.m_coords[0]);
        m_coords[1] = psa::add(m_coords[1], rhs.m_coords[1]);
        m_coords[2] = psa::add(m_coords[2], rhs.m_coords[2]);
        m_coords[3] = psa::add(m_coords[3], rhs.m_coords[3]);
    }

    bool operator==(const HVector& rhs) const
    {
        return psa::isequal(rhs.m_coords[0], m_coords[0]) &&
            psa::isequal(rhs.m_coords[1], m_coords[1]) &&
            psa::isequal(rhs.m_coords[2], m_coords[2]) &&
            psa::isequal(rhs.m_coords[3], m_coords[3]);
    }
    bool operator!=(const HVector& rhs) const { return !(rhs == *this); }

    HVector& operator+=(const HVector& rhs)
    {
        m_coords[0] += rhs.m_coords[0];
        m_coords[1] += rhs.m_coords[1];
        m_coords[2] += rhs.m_coords[2];
        m_coords[3] += rhs.m_coords[3];

        return *this;
    }
    HVector operator+(const HVector& rhs) const
    {
        return HVector{m_coords[0] + rhs.m_coords[0],
            m_coords[1] + rhs.m_coords[1],
            m_coords[2] + rhs.m_coords[2],
            m_coords[3] + rhs.m_coords[3]};
    }

    HVector& operator-=(const HVector& rhs)
    {
        m_coords[0] -= rhs.m_coords[0];
        m_coords[1] -= rhs.m_coords[1];
        m_coords[2] -= rhs.m_coords[2];
        m_coords[3] -= rhs.m_coords[3];

        return *this;
    }
    HVector operator-(const HVector& rhs) const
    {
        return HVector{m_coords[0] - rhs.m_coords[0],
            m_coords[1] - rhs.m_coords[1],
            m_coords[2] - rhs.m_coords[2],
            m_coords[3] - rhs.m_coords[3]};
    }

private:
    void initialize(double x, double y, double z, double w)
    {
        m_coords[0] = x;
        m_coords[1] = y;
        m_coords[2] = z;
        m_coords[3] = w;
    }
    void initialize(const double* values)
    {
        this->initialize(values[0], values[1], values[2], values[3]);
    }

    std::array<double, 4> m_coords;
};

} // namespace Core

#endif // CORE_HVECTOR_H
