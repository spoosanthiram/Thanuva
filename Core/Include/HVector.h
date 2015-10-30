/**
* Author: Saravanan Poosanthiram
* $LastChangedBy: ps $
* $LastChangedDate: 2015-04-01 16:35:03 -0400 (Wed, 01 Apr 2015) $
*/

#ifndef CORE_HVECTOR_H
#define CORE_HVECTOR_H

#include <vector>

#include "AlgoBase.h"

namespace Core {

class Vector3d;

/**
* HVector is an abstraction of homogeneous vector used in graphics.
* see http://en.wikipedia.org/wiki/Homogeneous_coordinates
*/
class HVector {
public:
    static const int kDimension = 4;

public:
    HVector() { this->initialize(0.0, 0.0, 0.0, 0.0); }
    HVector(double x, double y, double z, double w) { this->initialize(x, y, z, w); }
    explicit HVector(const double* values) { this->initialize(values); }
    explicit HVector(const Vector3d& v);

    double x() const { return m_elements[0]; }
    double y() const { return m_elements[1]; }
    double z() const { return m_elements[2]; }
    double w() const { return m_elements[3]; }
    double operator[] (int i) const { return m_elements[i]; }
    const double* data() const { return m_elements; }

    void setX(double x) { m_elements[0] = x; }
    void setY(double y) { m_elements[1] = y; }
    void setZ(double z) { m_elements[2] = z; }
    void setW(double w) { m_elements[3] = w; }
    void assign(double x, double y, double z, double w) { this->initialize(x, y, z, w); }
    void assign(std::vector<double>::const_iterator it)
    {
        this->initialize(*it, *(it + 1), *(it + 2), *(it + 3));
    }
    void assign(const Vector3d& v);

    bool operator==(const HVector& rhs) const
    {
        return psa::isequal(rhs.m_elements[0], m_elements[0])
            && psa::isequal(rhs.m_elements[1], m_elements[1])
            && psa::isequal(rhs.m_elements[2], m_elements[2])
            && psa::isequal(rhs.m_elements[3], m_elements[3]);
    }
    bool operator!=(const HVector& rhs) const { return !(rhs == *this); }
    HVector operator+(const HVector& rhs) const { HVector result(*this); result.add(rhs); return result; }
    HVector operator-(const HVector& rhs) const { HVector result(*this); result.subtract(rhs); return result; }

    void scale(double scalar)
    {
        m_elements[0] *= scalar; m_elements[1] *= scalar; m_elements[2] *= scalar; m_elements[3] *= scalar;
    }
    void scaleInfinite(double scalar)
    {
        m_elements[0] = psa::multiply(scalar, m_elements[0]);
        m_elements[1] = psa::multiply(scalar, m_elements[1]);
        m_elements[2] = psa::multiply(scalar, m_elements[2]);
        m_elements[3] = psa::multiply(scalar, m_elements[3]);
    }
    HVector& operator+=(const HVector& rhs) { this->add(rhs); return *this; }
    HVector& operator-=(const HVector& rhs) { this->subtract(rhs); return *this; }
    void addInfinite(const HVector& rhs)
    {
        m_elements[0] = psa::add(m_elements[0], rhs.m_elements[0]);
        m_elements[1] = psa::add(m_elements[1], rhs.m_elements[1]);
        m_elements[2] = psa::add(m_elements[2], rhs.m_elements[2]);
        m_elements[3] = psa::add(m_elements[3], rhs.m_elements[3]);
    }

private:
    void initialize(double x, double y, double z, double w)
    {
        m_elements[0] = x;
        m_elements[1] = y;
        m_elements[2] = z;
        m_elements[3] = w;
    }
    void initialize(const double* values)
    {
        if (nullptr == values)
            this->initialize(0.0, 0.0, 0.0, 0.0);
        else
            this->initialize(values[0], values[1], values[2], values[3]);
    }
    void add(const HVector& hv)
    {
        m_elements[0] += hv.m_elements[0];
        m_elements[1] += hv.m_elements[1];
        m_elements[2] += hv.m_elements[2];
        m_elements[3] += hv.m_elements[3];
    }
    void subtract(const HVector& hv)
    {
        m_elements[0] -= hv.m_elements[0];
        m_elements[1] -= hv.m_elements[1];
        m_elements[2] -= hv.m_elements[2];
        m_elements[3] -= hv.m_elements[3];
    }

    double m_elements[4];
};

} // namespace Core

#endif // CORE_HVECTOR_H
