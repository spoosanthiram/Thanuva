/*
 * Core: Common Code used by other modules of Thanuva
 *
 * Copyright 2016, Saravanan Poosanthiram
 * All rights reserved.
 */

#include "Matrix4x4.h"

#include <regex>
#include <sstream>

#include <AlgoBase.h>
#ifdef UNIT_TEST
#include <gtest/gtest.h>
#endif

#include "CoreDef.h"
#include "HPoint.h"
#include "HVector.h"
#include "Matrix3x3.h"
#include "Quaternion.h"
#include "Vector3d.h"

namespace Core {

Matrix4x4 Matrix4x4::frustum(double left, double right,
                             double bottom, double top,
                             double near, double far)
{
    Matrix4x4 projection;
    projection.makeZero();

    projection.m_elements[0] = (2.0 * near) / (right - left);
    projection.m_elements[5] = (2.0 * near) / (top - bottom);
    projection.m_elements[8] = (right + left) / (right - left);
    projection.m_elements[9] = (top + bottom) / (top - bottom);
    projection.m_elements[10] = -(far + near) / (far - near);
    projection.m_elements[11] = -1;
    projection.m_elements[14] = -(2.0 * far * near) / (far - near);

    return projection;
}

Matrix4x4 Matrix4x4::perspective(double fovy, double aspect, double near, double far)
{
    Matrix4x4 projection;
    projection.makeZero();

    double q = 1.0 / std::tan(0.5 * fovy * psa::kRadianMultiplier);
    projection.m_elements[0] = q / aspect;
    projection.m_elements[5] = q;
    projection.m_elements[10] = (near + far) / (near - far);
    projection.m_elements[11] = -1.0;
    projection.m_elements[14] = (2.0 * near * far) / (near - far);

    return projection;
}

Matrix4x4 Matrix4x4::ortho(double left, double right, double bottom, double top, double near, double far)
{
    Matrix4x4 projection;
    projection.makeZero();

    projection.m_elements[0] = 2.0 / (right - left);
    projection.m_elements[5] = 2.0 / (top - bottom);
    projection.m_elements[10] = -2.0 / (far - near);
    projection.m_elements[12] = -(right + left) / (right - left);
    projection.m_elements[13] = -(top + bottom) / (top - bottom);
    projection.m_elements[14] = -(far + near) / (far - near);
    projection.m_elements[15] = 1.0;

    return projection;
}

Matrix4x4::Matrix4x4(double val0, double val1, double val2, double val3
    , double val4, double val5, double val6, double val7
    , double val8, double val9, double val10, double val11
    , double val12, double val13, double val14, double val15)
    : m_elements{}
{
    m_elements[0] = val0;
    m_elements[1] = val1;
    m_elements[2] = val2;
    m_elements[3] = val3;

    m_elements[4] = val4;
    m_elements[5] = val5;
    m_elements[6] = val6;
    m_elements[7] = val7;

    m_elements[8] = val8;
    m_elements[9] = val9;
    m_elements[10] = val10;
    m_elements[11] = val11;

    m_elements[12] = val12;
    m_elements[13] = val13;
    m_elements[14] = val14;
    m_elements[15] = val15;
}

void Matrix4x4::data(float* values) const
{
    for (std::size_t i = 0; i < m_elements.size(); ++i)
        values[i] = static_cast<float>(m_elements[i]);
}

std::string Matrix4x4::str() const
{
    std::ostringstream resultStream;
    for (auto value : m_elements)
        resultStream << value << " ";
    std::string result = resultStream.str();
    result.pop_back();
    return result;
}

void Matrix4x4::setColumn(int icolumn, double val0, double val1, double val2, double val3)
{
    m_elements[icolumn * kNRows] = val0;
    m_elements[icolumn * kNRows + 1] = val1;
    m_elements[icolumn * kNRows + 2] = val2;
    m_elements[icolumn * kNRows + 3] = val3;
}

void Matrix4x4::setColumn(int icolumn, const double* values)
{
    m_elements[icolumn * kNRows] = values[0];
    m_elements[icolumn * kNRows + 1] = values[1];
    m_elements[icolumn * kNRows + 2] = values[2];
    m_elements[icolumn * kNRows + 3] = values[3];
}

void Matrix4x4::setColumn(int icolumn, const HPoint& hp)
{
    m_elements[icolumn * kNRows] = hp[0];
    m_elements[icolumn * kNRows + 1] = hp[1];
    m_elements[icolumn * kNRows + 2] = hp[2];
    m_elements[icolumn * kNRows + 3] = hp[3];
}

void Matrix4x4::setColumn(int icolumn, const HVector& hv)
{
    m_elements[icolumn * kNRows] = hv[0];
    m_elements[icolumn * kNRows + 1] = hv[1];
    m_elements[icolumn * kNRows + 2] = hv[2];
    m_elements[icolumn * kNRows + 3] = hv[3];
}

void Matrix4x4::set(const std::string& str)
{
    std::regex re{kWhitespaceRegEx};

    std::size_t k = 0;
    auto it = std::sregex_token_iterator(str.begin(), str.end(), re, -1);
    while (k < m_elements.size() && it != std::sregex_token_iterator()) {
        m_elements[k] = std::stod((*it).str());
        ++it;
        ++k;
    }
}

void Matrix4x4::makeIdentity()
{
    this->makeZero();
    for (unsigned int i = 0; i < kNColumns; ++i)
        m_elements[i*kNRows + i] = 1.0;
}

void Matrix4x4::transpose()
{
    std::swap(m_elements[1], m_elements[4]);
    std::swap(m_elements[2], m_elements[8]);
    std::swap(m_elements[3], m_elements[12]);

    std::swap(m_elements[6], m_elements[9]);
    std::swap(m_elements[7], m_elements[13]);

    std::swap(m_elements[11], m_elements[14]);
}

void Matrix4x4::invert()
{
    const double* elements = m_elements.data();

    Matrix3x3 mat;

    mat.setColumn(0, elements);
    mat.setColumn(1, &elements[4]);
    mat.setColumn(2, &elements[8]);

    mat.invert();

    const double* adata = mat.data();
    this->setColumn(0, adata[0], adata[1], adata[2], 0.0);
    this->setColumn(1, adata[3], adata[4], adata[5], 0.0);
    this->setColumn(2, adata[6], adata[7], adata[8], 0.0);

    mat = -1.0 * mat;
    Vector3d b{&elements[12]};
    b = mat * b;

    this->setColumn(3, HVector{b});
}

void Matrix4x4::scale(double x, double y, double z)
{
    if ((x < 0.0 || psa::iszero(x) || psa::isequal(x, 1.0)) &&
        (y < 0.0 || psa::iszero(y) || psa::isequal(y, 1.0)) &&
        (z < 0.0 || psa::iszero(z) || psa::isequal(z, 1.0)))
        return;

    Matrix4x4 scaleMatrix = Matrix4x4::identity();

    if (x < 0.0 || psa::iszero(x) || psa::isequal(x, 1.0))
        scaleMatrix.m_elements[0] = 1.0;
    else
        scaleMatrix.m_elements[0] = x;

    if (y < 0.0 || psa::iszero(y) || psa::isequal(y, 1.0))
        scaleMatrix.m_elements[kNRows + 1] = 1.0;
    else
        scaleMatrix.m_elements[kNRows + 1] = y;

    if (z < 0.0 || psa::iszero(z) || psa::isequal(z, 1.0))
        scaleMatrix.m_elements[2 * kNRows + 2] = 1.0;
    else
        scaleMatrix.m_elements[2 * kNRows + 2] = z;

    *this *= scaleMatrix;
}

void Matrix4x4::translate(double x, double y, double z)
{
    if (psa::iszero(x) && psa::iszero(y) && psa::iszero(z))
        return;

    Matrix4x4 translation = Matrix4x4::identity();

    if (!psa::iszero(x))
        translation.m_elements[12] = x;
    if (!psa::iszero(y))
        translation.m_elements[13] = y;
    if (!psa::iszero(z))
        translation.m_elements[14] = z;

    *this *= translation;
}

void Matrix4x4::rotateAlongX(double angle)
{
    if (psa::iszero(angle))
        return;

    angle = psa::normalizeAngle(angle);

    double cosValue = psa::cos(angle);
    double sinValue = psa::sin(angle);

    Matrix4x4 rotation = Matrix4x4::identity();
    rotation.m_elements[5] = cosValue;
    rotation.m_elements[6] = sinValue;
    rotation.m_elements[9] = -sinValue;
    rotation.m_elements[10] = cosValue;

    *this *= rotation;
}

void Matrix4x4::rotateAlongY(double angle)
{
    if (psa::iszero(angle))
        return;

    angle = psa::normalizeAngle(angle);

    double cosValue = psa::cos(angle);
    double sinValue = psa::sin(angle);

    Matrix4x4 rotation = Matrix4x4::identity();
    rotation.m_elements[0] = cosValue;
    rotation.m_elements[2] = -sinValue;
    rotation.m_elements[8] = sinValue;
    rotation.m_elements[10] = cosValue;

    *this *= rotation;
}

void Matrix4x4::rotateAlongZ(double angle)
{
    if (psa::iszero(angle))
        return;

    angle = psa::normalizeAngle(angle);

    double cosValue = psa::cos(angle);
    double sinValue = psa::sin(angle);

    Matrix4x4 rotation = Matrix4x4::identity();
    rotation.m_elements[0] = cosValue;
    rotation.m_elements[1] = sinValue;
    rotation.m_elements[4] = -sinValue;
    rotation.m_elements[5] = cosValue;

    *this *= rotation;
}

void Matrix4x4::rotate(const Quaternion& q)
{
    double xx = q.x() * q.x();
    double xy = q.x() * q.y();
    double xz = q.x() * q.z();
    double xw = q.x() * q.w();

    double yy = q.y() * q.y();
    double yz = q.y() * q.z();
    double yw = q.y() * q.w();

    double zz = q.z() * q.z();
    double zw = q.z() * q.w();

    Matrix4x4 rotation;

    rotation.m_elements[0] = 1.0 - 2.0 * (yy + zz);
    rotation.m_elements[1] = 2.0 * (xy + zw);
    rotation.m_elements[2] = 2.0 * (xz - yw);
    rotation.m_elements[3] = 0.0;

    rotation.m_elements[4] = 2.0 * (xy - zw);
    rotation.m_elements[5] = 1.0 - 2.0 * (xx + zz);
    rotation.m_elements[6] = 2.0 * (yz + xw);
    rotation.m_elements[7] = 0.0;

    rotation.m_elements[8] = 2.0 * (xz + yw);
    rotation.m_elements[9] = 2.0 * (yz - xw);
    rotation.m_elements[10] = 1.0 - 2.0 * (xx + yy);
    rotation.m_elements[11] = 0.0;

    rotation.m_elements[12] = 0.0;
    rotation.m_elements[13] = 0.0;
    rotation.m_elements[14] = 0.0;
    rotation.m_elements[15] = 1.0;

    *this *= rotation;
}

bool Matrix4x4::operator==(const Matrix4x4& rhs) const
{
    bool equal = true;
    for (unsigned int i = 0; i < m_elements.size(); ++i) {
        if (!psa::isequal(m_elements[i], rhs.m_elements[i])) {
            equal = false;
            break;
        }
    }
    return equal;
}

Matrix4x4 Matrix4x4::operator*(const Matrix4x4& rhs) const
{
    Matrix4x4 result = Matrix4x4::zero();
    for (unsigned int i = 0; i < kNColumns; ++i) {
        for (unsigned int j = 0; j < kNRows; ++j) {
            for (unsigned int k = 0; k < kNRows; ++k) {
                result.m_elements[(i * kNRows) + k] +=
                        m_elements[(j * kNRows) + k] * rhs.m_elements[(i * kNRows) + j];
            }
        }
    }
    return result;
}

HPoint Matrix4x4::operator*(const HPoint& rhs) const
{
    HPoint result;
    for (std::size_t i = 0; i < result.size(); ++i)
    {
        HVector v2(&m_elements[i * kNRows]);
        v2.scale(rhs[i]);
        result += v2;
    }
    return result;
}

HPoint Matrix4x4::multiplyInfinite(const HPoint& rhs) const
{
    HPoint result;
    for (std::size_t i = 0; i < result.size(); ++i)
    {
        HVector v2(&m_elements[i * kNRows]);
        v2.scaleInfinite(rhs[i]);
        result.addInfinite(v2);
    }
    return result;
}

#ifdef UNIT_TEST

TEST(Matrix4x4Test, Zero)
{
    Matrix4x4 zero = Matrix4x4::zero();
    std::array<double, 16> zeroValues{};
    zeroValues.fill(0.0);
    Matrix4x4 expected{zeroValues};
    EXPECT_EQ(expected, zero);
}

TEST(Matrix4x4Test, Identity)
{
    Matrix4x4 eye = Matrix4x4::identity();
    std::array<double, 16> eyeValues{};
    eyeValues.fill(0.0);
    eyeValues[0] = 1.0;
    eyeValues[5] = 1.0;
    eyeValues[10] = 1.0;
    eyeValues[15] = 1.0;
    Matrix4x4 expected{eyeValues};
    EXPECT_EQ(expected, eye);
}

TEST(Matrix4x4Test, Transpose)
{
    Matrix4x4 mat{std::array<double, 16>
        {0.8147, 0.9058, 0.1270, 0.9134,
         0.6324, 0.0975, 0.2785, 0.5469,
         0.9575, 0.9649, 0.1576, 0.9706,
         0.9572, 0.4854, 0.8003, 0.1419}};
    Matrix4x4 expected{std::array<double, 16>
        {0.8147, 0.6324, 0.9575, 0.9572,
         0.9058, 0.0975, 0.9649, 0.4854,
         0.1270, 0.2785, 0.1576, 0.8003,
         0.9134, 0.5469, 0.9706, 0.1419}};
    mat.transpose();
    EXPECT_EQ(expected, mat);
}

TEST(Matrix4x4Test, Inverse)
{
    Matrix4x4 mat{std::array<double, 16>
        {1.0,  0.0,               0.0,               0.0,
         0.0,  1.732050807568877, 1.5,               0.0,
         0.0, -1.0,               2.598076211353316, 0.0,
         0.0,  0.0,               0.0,               1.0}};
    Matrix4x4 expected{std::array<double, 16>
        {1.0, 0.0,                  0.0,                 0.0,
         0.0, 0.43301270189221941, -0.25,                0.0,
         0.0, 0.16666666666666669,  0.28867513459481287, 0.0,
         0.0, 0.0,                  0.0,                 1.0}};
    mat.invert();
    EXPECT_EQ(expected, mat);
}

TEST(Matrix4x4Test, Rotate)
{
    Matrix4x4 mat{};
    mat.makeIdentity();
    mat.rotateAlongX(45);

    Matrix4x4 mat2{};
    mat2.makeIdentity();
    mat2.rotate(Quaternion{Vector3d{1.0, 0.0, 0.0}, 45});

    EXPECT_EQ(mat, mat2);
}

#endif // UNIT_TEST

} // namespace Core
