/*
 * Core: Common Code used by other modules of Thanuva
 *
 * Copyright 2016, Saravanan Poosanthiram
 * All rights reserved.
 */

#ifndef CORE_MATRIX4X4_H
#define CORE_MATRIX4X4_H

#include <array>

namespace Core {

class HVector;
class Quaternion;

/**
* Column based matrix
*/
class Matrix4x4
{
public:
    static const int kNRows = 4;
    static const int kNColumns = 4;

public:
    static Matrix4x4 zero()
    {
        Matrix4x4 z;
        z.makeZero();
        return z;
    }

    static Matrix4x4 identity()
    {
        Matrix4x4 eye;
        eye.makeIdentity();
        return eye;
    }

    static Matrix4x4 frustum(double left, double right,
                             double bottom, double top,
                             double near, double far);
    static Matrix4x4 perspective(double fovy, double aspect, double near, double far);

public:
    Matrix4x4() : m_elements() {}
    Matrix4x4(double val0, double val1, double val2, double val3
        , double val4, double val5, double val6, double val7
        , double val8, double val9, double val10, double val11
        , double val12, double val13, double val14, double val15);
    Matrix4x4(const std::array<double, kNRows * kNColumns>& values) : m_elements(values) {}
    Matrix4x4(const Matrix4x4& rhs) : m_elements(rhs.m_elements) {}
    Matrix4x4(Matrix4x4&& rhs) : m_elements(rhs.m_elements) {}

    Matrix4x4& operator=(const Matrix4x4& rhs)
    {
        if (this != &rhs)
            m_elements = rhs.m_elements;
        return *this;
    }
    Matrix4x4& operator=(Matrix4x4&& rhs)
    {
        if (this != &rhs)
            m_elements = rhs.m_elements;
        return *this;
    }

    double operator()(std::size_t irow, std::size_t icolumn) const
    {
        return m_elements[(icolumn * kNRows) + irow];
    }
    const double* data() const { return m_elements.data(); }
    void data(float* values) const;
    std::string str() const;

    void setValue(int irow, int icolumn, double value)
    {
        m_elements[(icolumn * kNRows) + irow] = value;
    }
    void setColumn(int icolumn, double val0, double val1, double val2, double val3);
    void setColumn(int icolumn, const double* values);
    void setColumn(int icolumn, const HVector& hv);
    void set(const std::string& str);

    void makeZero() { m_elements.fill(0.0); }
    void makeIdentity();
    void transpose();
    void invert();
    void scale(double x, double y, double z);
    void translate(double x, double y, double z);
    void rotateAlongX(double angle);
    void rotateAlongY(double angle);
    void rotateAlongZ(double angle);
    void rotate(const Quaternion& q);

    Matrix4x4& operator*=(const Matrix4x4& rhs)
    {
        *this = *this * rhs;
        return *this;
    }

    bool operator==(const Matrix4x4& rhs) const;
    bool operator!=(const Matrix4x4& rhs) const { return !(*this == rhs); }
    Matrix4x4 operator*(const Matrix4x4& rhs) const;
    HVector operator*(const HVector& rhs) const;
    HVector multiplyInfinite(const HVector& rhs) const;

protected:
    std::array<double, kNRows * kNColumns> m_elements;
};

} // namespace Core

#endif // CORE_MATRIX4X4_H
