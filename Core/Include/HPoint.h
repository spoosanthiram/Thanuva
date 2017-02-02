/*
 * Core: Common Code used by other modules of Thanuva
 *
 * Copyright 2016, Saravanan Poosanthiram
 * All rights reserved.
 */

#ifndef CORE_HPOINT_H
#define CORE_HPOINT_H

#include <array>
#include <string>

#include <AlgoBase.h>

#include "HVector.h"
#include "Point3d.h"

namespace Core {

/**
* The HPoint class is an abstraction of a point in homogeneous coordinate system.
* see http://en.wikipedia.org/wiki/Homogeneous_coordinates
*/
class HPoint
{
public:
    HPoint() { this->initialize(0.0, 0.0, 0.0, 0.0); }
    HPoint(double x, double y, double z, double w) { this->initialize(x, y, z, w); }
    explicit HPoint(const double* values) { this->initialize(values); }
    explicit HPoint(const Point3d& point)
    {
        this->initialize(point.x(), point.y(), point.z(), 1.0);
    }

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
    void assign(const Point3d& point)
    {
        this->initialize(point.x(), point.y(), point.z(), 1.0);
    }

    void addInfinite(const HVector& rhs)
    {
        m_coords[0] = psa::add(m_coords[0], rhs[0]);
        m_coords[1] = psa::add(m_coords[1], rhs[1]);
        m_coords[2] = psa::add(m_coords[2], rhs[2]);
        m_coords[3] = psa::add(m_coords[3], rhs[3]);
    }

    bool operator==(const HPoint& rhs) const
    {
        return psa::isequal(rhs.m_coords[0], m_coords[0]) &&
            psa::isequal(rhs.m_coords[1], m_coords[1]) &&
            psa::isequal(rhs.m_coords[2], m_coords[2]) &&
            psa::isequal(rhs.m_coords[3], m_coords[3]);
    }
    bool operator!=(const HPoint& rhs) const { return !(rhs == *this); }

    HPoint& operator+=(const HVector& hv)
    {
        m_coords[0] += hv[0];
        m_coords[1] += hv[1];
        m_coords[2] += hv[2];
        m_coords[3] += hv[3];

        return *this;
    }
    HPoint operator+(const HVector& hv) const
    {
        return HPoint{m_coords[0] + hv[0], m_coords[1] + hv[1], m_coords[2] + hv[2], m_coords[3] + hv[3]};
    }

    HPoint& operator-=(const HVector& hv) {
        m_coords[0] -= hv[0];
        m_coords[1] -= hv[1];
        m_coords[2] -= hv[2];
        m_coords[3] -= hv[3];

        return *this;
    }
    HPoint operator-(const HVector& hv) const
    {
        return HPoint{m_coords[0] - hv[0], m_coords[1] - hv[1], m_coords[2] - hv[2], m_coords[3] - hv[3]};
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

#endif // CORE_HPOINT_H
