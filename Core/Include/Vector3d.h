/*
 * Core: Common Code used by other modules of Thanuva
 *
 * Copyright 2016, Saravanan Poosanthiram
 * All rights reserved.
 */

#ifndef CORE_VECTOR3D_H
#define CORE_VECTOR3D_H

#include <array>
#include <string>

#include <AlgoBase.h>

namespace Core {

class HVector;

class Vector3d
{
public:
    Vector3d() { this->initialize(0.0, 0.0, 0.0); }
    Vector3d(float x, float y, float z) { this->initialize(x, y, z); }
    Vector3d(double x, double y, double z) { this->initialize(x, y, z); }
    explicit Vector3d(const float* values) { this->initialize(values); }
    explicit Vector3d(const double* values) { this->initialize(values); }
    explicit Vector3d(const HVector& hv);

    double x() const { return m_elements[0]; }
    double y() const { return m_elements[1]; }
    double z() const { return m_elements[2]; }
    double operator[] (std::size_t i) const { return m_elements[i]; }
    std::size_t size() const { return m_elements.size(); }
    const double* data() const { return m_elements.data(); }
    std::string str() const;

    void setX(double x) { m_elements[0] = x; }
    void setY(double y) { m_elements[1] = y; }
    void setZ(double z) { m_elements[2] = z; }

    void assign(float x, float y, float z) { this->initialize(x, y, z); }
    void assign(double x, double y, double z) { this->initialize(x, y, z); }
    void assign(const float* values) { this->initialize(values); }
    void assign(const double* values) { this->initialize(values); }
    void assign(const HVector& hv);

    bool iszero() const
    {
        return psa::iszero(m_elements[0]) &&
            psa::iszero(m_elements[1]) &&
            psa::iszero(m_elements[2]);
    }
    double dot(const Vector3d& rhs) const
    {
        return (m_elements[0] * rhs.m_elements[0]) +
            (m_elements[1] * rhs.m_elements[1]) +
            (m_elements[2] * rhs.m_elements[2]);
    }
    Vector3d cross(const Vector3d& rhs) const
    {
        return Vector3d(
            m_elements[1] * rhs.m_elements[2] - rhs.m_elements[1] * m_elements[2],
            -(m_elements[0] * rhs.m_elements[2] - rhs.m_elements[0] * m_elements[2]),
            m_elements[0] * rhs.m_elements[1] - rhs.m_elements[0] * m_elements[1]);
    }
    double norm() const { return std::sqrt(this->dot(*this)); }
    double distance(const Vector3d& rhs) const { return (*this - rhs).norm(); }

    void setNorm(double norm)
    {
        double oldNorm = this->norm();
        if (oldNorm != 0 && norm != oldNorm)
            this->scale(norm / oldNorm);
    }
    void normalize() { this->scale(1.0 / this->norm()); }
    void scale(double scalar)
    {
        m_elements[0] *= scalar;
        m_elements[1] *= scalar;
        m_elements[2] *= scalar;
    }
    void scaleInfinite(double scalar)
    {
        m_elements[0] = psa::multiply(scalar, m_elements[0]);
        m_elements[1] = psa::multiply(scalar, m_elements[1]);
        m_elements[2] = psa::multiply(scalar, m_elements[2]);
    }
    void addInfinite(const Vector3d& rhs)
    {
        m_elements[0] = psa::add(m_elements[0], rhs.m_elements[0]);
        m_elements[1] = psa::add(m_elements[1], rhs.m_elements[1]);
        m_elements[2] = psa::add(m_elements[2], rhs.m_elements[2]);
    }

    bool operator==(const Vector3d& rhs) const
    {
        return psa::isequal(rhs.m_elements[0], m_elements[0])
                && psa::isequal(rhs.m_elements[1], m_elements[1])
                && psa::isequal(rhs.m_elements[2], m_elements[2]);
    }
    bool operator!=(const Vector3d& rhs) const { return !(rhs == *this); }

    Vector3d& operator+=(const Vector3d& rhs)
    {
        this->add(rhs);
        return *this;
    }
    Vector3d operator+(const Vector3d& rhs) const
    {
        Vector3d result(*this);
        result.add(rhs);
        return result;
    }

    Vector3d& operator-=(const Vector3d& rhs)
    {
        this->subtract(rhs);
        return *this;
    }
    Vector3d operator-(const Vector3d& rhs) const
    {
        Vector3d result(*this);
        result.subtract(rhs);
        return result;
    }

    friend Vector3d operator*(double scalar, const Vector3d& v)
    {
        Vector3d result(v);
        result.scale(scalar);
        return result;
    }
    friend Vector3d operator*(const Vector3d& v, double scalar) { return scalar * v; }

private:
    void initialize(double x, double y, double z)
    {
        m_elements[0] = x;
        m_elements[1] = y;
        m_elements[2] = z;
    }
    void initialize(const float* values)
    {
        this->initialize(values[0], values[1], values[2]);
    }
    void initialize(const double* values)
    {
        this->initialize(values[0], values[1], values[2]);
    }
    void add(const Vector3d& v)
    {
        m_elements[0] += v.m_elements[0];
        m_elements[1] += v.m_elements[1];
        m_elements[2] += v.m_elements[2];
    }
    void subtract(const Vector3d& v)
    {
        m_elements[0] -= v.m_elements[0];
        m_elements[1] -= v.m_elements[1];
        m_elements[2] -= v.m_elements[2];
    }

    std::array<double, 3> m_elements;
};

} // namespace Core

#endif // CORE_VECTOR3D_H
