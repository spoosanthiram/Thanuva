/*
 * Geometry: Geometry objects for Thanuva
 *
 * Copyright 2016, Saravanan Poosanthiram
 * All rights reserved.
 */

#include "Extent.h"

#include <algorithm>

#include <AlgoBase.h>
#ifdef UNIT_TEST
#include <gtest/gtest.h>
#endif

#include "HVector.h"
#include "Matrix4x4.h"

namespace Geometry {

void Extent::update(const Extent& extent)
{
    if (!psa::isNegativeInfinite(extent.m_xMin))
        m_xMin = std::min<double>(psa::isNegativeInfinite(m_xMin) ?
                                  std::numeric_limits<double>::infinity() : m_xMin, extent.m_xMin);
    if (!psa::isNegativeInfinite(extent.m_yMin))
        m_yMin = std::min<double>(psa::isNegativeInfinite(m_yMin) ?
                                  std::numeric_limits<double>::infinity() : m_yMin, extent.m_yMin);
    if (!psa::isNegativeInfinite(extent.m_zMin))
        m_zMin = std::min<double>(psa::isNegativeInfinite(m_zMin) ?
                                  std::numeric_limits<double>::infinity() : m_zMin, extent.m_zMin);

    if (!psa::isPositiveInfinite(extent.m_xMax))
        m_xMax = std::max<double>(psa::isPositiveInfinite(m_xMax) ?
                                  -std::numeric_limits<double>::infinity() : m_xMax, extent.m_xMax);
    if (!psa::isPositiveInfinite(extent.m_yMax))
        m_yMax = std::max<double>(psa::isPositiveInfinite(m_yMax) ?
                                  -std::numeric_limits<double>::infinity() : m_yMax, extent.m_yMax);
    if (!psa::isPositiveInfinite(extent.m_zMax))
        m_zMax = std::max<double>(psa::isPositiveInfinite(m_zMax) ?
                                  -std::numeric_limits<double>::infinity() : m_zMax, extent.m_zMax);

    if (m_xMin > m_xMax) m_xMin = m_xMax;
    if (m_yMin > m_yMax) m_yMin = m_yMax;
    if (m_zMin > m_zMax) m_zMin = m_zMax;
}

void Extent::transform(const Core::Matrix4x4& xformMatrix, CalculationModel model)
{
    if (this->isAnyInfinite())
        return;

    Core::HVector vertices[kNumBoxVertices];
    vertices[0].assign(m_xMin, m_yMin, m_zMin, 1.0);
    vertices[1].assign(m_xMax, m_yMin, m_zMin, 1.0);
    vertices[2].assign(m_xMax, m_yMax, m_zMin, 1.0);
    vertices[3].assign(m_xMin, m_yMax, m_zMin, 1.0);
    if (CalculationModel::Pyramid== model)
        vertices[4].assign(m_xMin + this->xLength() / 2.0, m_yMin + this->yLength() / 2.0, m_zMax, 1.0);
    else {
        vertices[4].assign(m_xMin, m_yMin, m_zMax, 1.0);
        vertices[5].assign(m_xMax, m_yMin, m_zMax, 1.0);
        vertices[6].assign(m_xMax, m_yMax, m_zMax, 1.0);
        vertices[7].assign(m_xMin, m_yMax, m_zMax, 1.0);
    }

    int nVertices = (CalculationModel::Pyramid == model) ? kNumPyramidVertices : kNumBoxVertices;

    for (int i = 0; i < nVertices; ++i)
        vertices[i] = xformMatrix * vertices[i];

    m_xMin = m_xMax = vertices[0].x();
    m_yMin = m_yMax = vertices[0].y();
    m_zMin = m_zMax = vertices[0].z();

    for (int i = 1; i < nVertices; ++i) {
        if (vertices[i].x() < m_xMin) m_xMin = vertices[i].x();
        if (vertices[i].x() > m_xMax) m_xMax = vertices[i].x();

        if (vertices[i].y() < m_yMin) m_yMin = vertices[i].y();
        if (vertices[i].y() > m_yMax) m_yMax = vertices[i].y();

        if (vertices[i].z() < m_zMin) m_zMin = vertices[i].z();
        if (vertices[i].z() > m_zMax) m_zMax = vertices[i].z();
    }
}

void Extent::transformInfinite(const Core::Matrix4x4& xformMatrix)
{
    if (psa::isInfinite(m_xMin) && psa::isInfinite(m_xMax)
            && psa::isInfinite(m_yMin) && psa::isInfinite(m_yMax)
            && psa::isInfinite(m_zMin) && psa::isInfinite(m_zMax)) // this NOT this->isAllInfinite()
        return;

    Core::HVector vertices[kNumBoxVertices];
    vertices[0].assign(m_xMin, m_yMin, m_zMin, 1.0);
    vertices[1].assign(m_xMax, m_yMin, m_zMin, 1.0);
    vertices[2].assign(m_xMax, m_yMax, m_zMin, 1.0);
    vertices[3].assign(m_xMin, m_yMax, m_zMin, 1.0);
    vertices[4].assign(m_xMin, m_yMin, m_zMax, 1.0);
    vertices[5].assign(m_xMax, m_yMin, m_zMax, 1.0);
    vertices[6].assign(m_xMax, m_yMax, m_zMax, 1.0);
    vertices[7].assign(m_xMin, m_yMax, m_zMax, 1.0);

    for (int i = 0; i < kNumBoxVertices; ++i)
        vertices[i] = xformMatrix.multiplyInfinite(vertices[i]);

    m_xMin = std::numeric_limits<double>::infinity();
    m_xMax = -std::numeric_limits<double>::infinity();
    m_yMin = std::numeric_limits<double>::infinity();
    m_yMax = -std::numeric_limits<double>::infinity();
    m_zMin = std::numeric_limits<double>::infinity();
    m_zMax = -std::numeric_limits<double>::infinity();

    for (int i = 0; i < kNumBoxVertices; ++i) {
        if (vertices[i].x() < m_xMin) m_xMin = vertices[i].x();
        if (vertices[i].x() > m_xMax) m_xMax = vertices[i].x();

        if (vertices[i].y() < m_yMin) m_yMin = vertices[i].y();
        if (vertices[i].y() > m_yMax) m_yMax = vertices[i].y();

        if (vertices[i].z() < m_zMin) m_zMin = vertices[i].z();
        if (vertices[i].z() > m_zMax) m_zMax = vertices[i].z();
    }
}

#ifdef UNIT_TEST

TEST(ExtentTest, Creation)
{
    // default construction
    Extent extent;
    EXPECT_EQ(-std::numeric_limits<double>::infinity(), extent.xMin());
    EXPECT_EQ(std::numeric_limits<double>::infinity(), extent.xMax());
    EXPECT_EQ(-std::numeric_limits<double>::infinity(), extent.yMin());
    EXPECT_EQ(std::numeric_limits<double>::infinity(), extent.yMax());
    EXPECT_EQ(-std::numeric_limits<double>::infinity(), extent.zMin());
    EXPECT_EQ(std::numeric_limits<double>::infinity(), extent.zMax());

    // construct with some values
    extent = Extent{-11.2, 3.0, -0.003, 10.004, 1000.4, 2.3e+9};
    EXPECT_EQ(-11.2, extent.xMin());
    EXPECT_EQ(3.0, extent.xMax());
    EXPECT_EQ(-0.003, extent.yMin());
    EXPECT_EQ(10.004, extent.yMax());
    EXPECT_EQ(1000.4, extent.zMin());
    EXPECT_EQ(2.3e+9, extent.zMax());
}

TEST(ExtentTest, SimpleOperations)
{
    double xmin = -1.37, xmax = 12.89;
    double ymin = 0.0, ymax = 1.2e-4;
    double zmin = 2031.0, zmax = 6032.2421;

    Extent extent{xmin, xmax, ymin, ymax, zmin, zmax};

    EXPECT_EQ((xmax - xmin), extent.xLength());
    EXPECT_EQ((ymax - ymin), extent.yLength());
    EXPECT_EQ((zmax - zmin), extent.zLength());

    EXPECT_EQ(xmin + (xmax - xmin) / 2.0, extent.xMid());
    EXPECT_EQ(ymin + (ymax - ymin) / 2.0, extent.yMid());
    EXPECT_EQ(zmin + (zmax - zmin) / 2.0, extent.zMid());

    EXPECT_EQ(xmin, extent.minimum());
    EXPECT_EQ(zmax, extent.maximum());
}

#endif // UNIT_TEST

} // namespace Geometry
