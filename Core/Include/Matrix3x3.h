/*
 * Core: Common Code used by other modules of Thanuva
 *
 * Copyright 2016, Saravanan Poosanthiram
 * All rights reserved.
 */

#ifndef CORE_MATRIX3X3_H
#define CORE_MATRIX3X3_H

#include <array>
#include <string>

namespace Core {

class Matrix4x4;
class Quaternion;
class Vector3d;

/**
* Column based 3x3 matrix
*/
class Matrix3x3
{
public:
    static const int kNRows = 3;
    static const int kNColumns = 3;

public:
    static Matrix3x3 zero()
    {
        Matrix3x3 z;
        z.makeZero();
        return z;
    }

    static Matrix3x3 identity()
    {
        Matrix3x3 eye;
        eye.makeIdentity();
        return eye;
    }

public:
    Matrix3x3() : m_elements{} {}
    Matrix3x3(double val0, double val1, double val2
        , double val3, double val4, double val5
        , double val6, double val7, double val8);
    Matrix3x3(const std::array<double, kNRows * kNColumns>& values) : m_elements(values) {}
    Matrix3x3(const Matrix4x4& rhs);
    Matrix3x3(const Matrix3x3& rhs) : m_elements(rhs.m_elements) {}
    Matrix3x3(Matrix3x3&& rhs) : m_elements(rhs.m_elements) {}

    Matrix3x3& operator=(const Matrix3x3& rhs)
    {
        if (this != &rhs)
            m_elements = rhs.m_elements;
        return *this;
    }
    Matrix3x3& operator=(Matrix3x3&& rhs)
    {
        if (this != &rhs)
            m_elements = rhs.m_elements;
        return *this;
    }

    double operator()(unsigned int irow, unsigned int icolumn) const { return m_elements[(icolumn * kNRows) + irow]; }
    const double* data() const { return m_elements.data(); }
    void data(float* values) const;
    std::string str() const;

    void setValue(int irow, int icolumn, double value) { m_elements[(icolumn * kNRows) + irow] = value; }
    void setColumn(int icolumn, double val0, double val1, double val2);
    void setColumn(int icolumn, const double* values);
    void setColumn(int icolumn, const Vector3d& v);
    void set(const std::string& str);

    double determinant() const;

    void makeZero() { m_elements.fill(0.0); }
    void makeIdentity();
    void transpose();
    void invert();
    void rotate(const Quaternion& q);

    bool operator==(const Matrix3x3& rhs) const;
    bool operator!=(const Matrix3x3& rhs) const { return !(*this == rhs); }
    Matrix3x3& operator*=(const Matrix3x3& rhs)
    {
        *this = *this * rhs;
        return *this;
    }
    Matrix3x3 operator+(const Matrix3x3& rhs) const;
    Matrix3x3 operator*(const Matrix3x3& rhs) const;
    Vector3d operator*(const Vector3d& rhs) const;

    friend Matrix3x3 operator*(double scalar, const Matrix3x3& m);

protected:
    std::array<double, kNRows * kNColumns> m_elements;
};

} // namespace Core

#endif // CORE_MATRIX3X3_H
