/**
* Author: Saravanan Poosanthiram
* $LastChangedBy: ps $
* $LastChangedDate: 2015-10-23 21:50:42 -0400 (Fri, 23 Oct 2015) $
*/

#include "Matrix3x3.h"

#include <regex>
#include <sstream>

#include "AlgoBase.h"

#ifdef UNIT_TEST
#include "gtest/gtest.h"
#endif

#include "CoreDef.h"
#include "Matrix4x4.h"
#include "Quaternion.h"
#include "Vector3d.h"

namespace Core {

Matrix3x3::Matrix3x3(double val0, double val1, double val2
    , double val3 , double val4, double val5
    , double val6 , double val7, double val8)
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
}

Matrix3x3::Matrix3x3(const Matrix4x4& rhs)
{
    const double* elements = rhs.data();

    this->setColumn(0, elements);
    this->setColumn(1, &elements[4]);
    this->setColumn(2, &elements[8]);
}

void Matrix3x3::data(float* values) const
{
    for (std::size_t i = 0; i < m_elements.size(); ++i)
        values[i] = static_cast<float>(m_elements[i]);
}

std::string Matrix3x3::str() const
{
    std::ostringstream resultStream;
    for (auto value : m_elements)
        resultStream << value << " ";
    std::string result = resultStream.str();
    result.pop_back();
    return result;
}

void Matrix3x3::setColumn(int icolumn, double val0, double val1, double val2)
{
    m_elements[icolumn * kNRows] = val0;
    m_elements[icolumn * kNRows + 1] = val1;
    m_elements[icolumn * kNRows + 2] = val2;
}

void Matrix3x3::setColumn(int icolumn, const double* values)
{
    m_elements[icolumn * kNRows] = values[0];
    m_elements[icolumn * kNRows + 1] = values[1];
    m_elements[icolumn * kNRows + 2] = values[2];
}

void Matrix3x3::setColumn(int icolumn, const Vector3d& v)
{
    m_elements[icolumn * kNRows] = v[0];
    m_elements[icolumn * kNRows + 1] = v[2];
    m_elements[icolumn * kNRows + 2] = v[3];
}

void Matrix3x3::set(const std::string& str)
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

double Matrix3x3::determinant() const
{
    const double* elements = m_elements.data();

    Vector3d x0{elements};
    Vector3d x1{&elements[3]};
    Vector3d x2{&elements[6]};

    return x0.dot(x1.cross(x2));
}

void Matrix3x3::makeIdentity()
{
    this->makeZero();
    for (unsigned int i = 0; i < kNColumns; ++i)
        m_elements[i*kNRows + i] = 1.0;
}

void Matrix3x3::transpose()
{
    std::swap(m_elements[1], m_elements[3]);
    std::swap(m_elements[2], m_elements[6]);

    std::swap(m_elements[5], m_elements[7]);
}

void Matrix3x3::invert()
{
    const double* elements = m_elements.data();

    Vector3d x0{elements};
    Vector3d x1{&elements[3]};
    Vector3d x2{&elements[6]};

    Vector3d r0 = x1.cross(x2);
    Vector3d r1 = x2.cross(x0);
    Vector3d r2 = x0.cross(x1);

    double oneOverDet = 1.0 / this->determinant();

    r0.scale(oneOverDet);
    r1.scale(oneOverDet);
    r2.scale(oneOverDet);

    m_elements[0] = r0.x();
    m_elements[1] = r1.x();
    m_elements[2] = r2.x();

    m_elements[3] = r0.y();
    m_elements[4] = r1.y();
    m_elements[5] = r2.y();

    m_elements[6] = r0.z();
    m_elements[7] = r1.z();
    m_elements[8] = r2.z();
}

void Matrix3x3::rotate(const Quaternion& q)
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

    Matrix3x3 rotation;

    rotation.m_elements[0] = 1.0 - 2.0 * (yy + zz);
    rotation.m_elements[1] = 2.0 * (xy + zw);
    rotation.m_elements[2] = 2.0 * (xz - yw);

    rotation.m_elements[3] = 2.0 * (xy - zw);
    rotation.m_elements[4] = 1.0 - 2.0 * (xx + zz);
    rotation.m_elements[5] = 2.0 * (yz + xw);

    rotation.m_elements[6] = 2.0 * (xz + yw);
    rotation.m_elements[7] = 2.0 * (yz - xw);
    rotation.m_elements[8] = 1.0 - 2.0 * (xx + yy);

    *this *= rotation;
}

bool Matrix3x3::operator==(const Matrix3x3& rhs) const
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

Matrix3x3 Matrix3x3::operator+(const Matrix3x3& rhs) const
{
    Matrix3x3 result{};
    for (std::size_t i = 0; i < m_elements.size(); ++i)
        result.m_elements[i] = m_elements[i] + rhs.m_elements[i];
    return result;
}

Matrix3x3 Matrix3x3::operator*(const Matrix3x3& rhs) const
{
    Matrix3x3 result = Matrix3x3::zero();
    for (unsigned int i = 0; i < kNColumns; ++i) {
        for (unsigned int j = 0; j < kNRows; ++j) {
            for (unsigned int k = 0; k < kNRows; ++k)
                result.m_elements[(i * kNRows) + k] += m_elements[(j * kNRows) + k] * rhs.m_elements[(i * kNRows) + j];
        }
    }
    return result;
}

Vector3d Matrix3x3::operator*(const Vector3d& rhs) const
{
    Vector3d result;
    for (unsigned int i = 0; i < Vector3d::kDimension; ++i) {
        Vector3d v2(&m_elements[i * kNRows]);
        v2.scale(rhs[i]);
        result += v2;
    }
    return result;
}

Matrix3x3 operator*(double scalar, const Matrix3x3& m)
{
    Matrix3x3 result;
    for (std::size_t i = 0; i < result.m_elements.size(); ++i)
        result.m_elements[i] = scalar * m.m_elements[i];
    return result;
}

#ifdef UNIT_TEST

TEST(Matrix3x3Test, Zero)
{
    Matrix3x3 zero = Matrix3x3::zero();
    std::array<double, 9> zeroValues{};
    zeroValues.fill(0.0);
    Matrix3x3 expected{zeroValues};
    EXPECT_EQ(expected, zero);
}

TEST(Matrix3x3Test, Identity)
{
    Matrix3x3 eye = Matrix3x3::identity();
    std::array<double, 9> eyeValues{};
    eyeValues.fill(0.0);
    eyeValues[0] = 1.0;
    eyeValues[4] = 1.0;
    eyeValues[8] = 1.0;
    Matrix3x3 expected{eyeValues};
    EXPECT_EQ(expected, eye);
}

TEST(Matrix3x3Test, Determinant)
{
    Matrix3x3 mat{std::array<double, 9>
        {0.814723, 0.905791, 0.126986, 0.913375, 0.632359, 0.097540, 0.278498, 0.546881, 0.957506}};
    EXPECT_EQ(-0.2766529971f, static_cast<float>(mat.determinant()));
}

TEST(Matrix3x3Test, Inverse)
{
    Matrix3x3 mat{std::array<double, 9>{{0.814723686393179, 0.905791937075619, 0.126986816293506, 0.913375856139019,
                0.632359246225410, 0.097540404999410, 0.278498218867048, 0.546881519204984, 0.957506835434298}}};
    Matrix3x3 expected{std::array<double, 9>{{-1.9957973611559359, 2.8839464655651592, -0.029097811479364084,
                3.0630278247219205, -2.6919472964268878, -0.13199960300229843,
                -1.1689601143598116, 0.69869190314284257, 1.1282340680150791}}};
    mat.invert();
    EXPECT_EQ(expected, mat);
}

#endif // UNIT_TEST

} // namespace Core
