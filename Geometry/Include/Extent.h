/*
 * Geometry: Geometry objects for Thanuva
 *
 * Copyright 2016, Saravanan Poosanthiram
 * All rights reserved.
 */

#ifndef GEOMETRY_EXTENT_H
#define GEOMETRY_EXTENT_H

#include <limits>

#include "Point3d.h"

namespace Core { class Matrix4x4; }

namespace Geometry {

class Extent
{
public:
    enum class CalculationModel
    {
        Box,
        Pyramid
    };

public:
    Extent()
        : m_xMin{-std::numeric_limits<double>::infinity()}
        , m_xMax{std::numeric_limits<double>::infinity()}
        , m_yMin{-std::numeric_limits<double>::infinity()}
        , m_yMax{std::numeric_limits<double>::infinity()}
        , m_zMin{-std::numeric_limits<double>::infinity()}
        , m_zMax{std::numeric_limits<double>::infinity()}
    {}
    Extent(double xMin, double xMax, double yMin, double yMax, double zMin, double zMax)
        : m_xMin{xMin}
        , m_xMax{xMax}
        , m_yMin{yMin}
        , m_yMax{yMax}
        , m_zMin{zMin}
        , m_zMax{zMax}
    {}

    double xMin() const { return m_xMin; }
    double xMax() const { return m_xMax; }
    double yMin() const { return m_yMin; }
    double yMax() const { return m_yMax; }
    double zMin() const { return m_zMin; }
    double zMax() const { return m_zMax; }
    double xLength() const { return m_xMax - m_xMin; }
    double yLength() const { return m_yMax - m_yMin; }
    double zLength() const { return m_zMax - m_zMin; }
    double xMid() const { return m_xMin + this->xLength() / 2.0; }
    double yMid() const { return m_yMin + this->yLength() / 2.0; }
    double zMid() const { return m_zMin + this->zLength() / 2.0; }
    double minimum() const
    {
        double minVal = (m_xMin < m_yMin) ? m_xMin : m_yMin;
        return (m_zMin < minVal) ? m_zMin : minVal;
    }
    double maximum() const
    {
        double maxVal = (m_xMax > m_yMax) ? m_xMax : m_yMax;
        return (m_zMax > maxVal) ? m_zMax : maxVal;
    }
    double minLength() const
    {
        double length = this->xLength();
        if (this->yLength() < length)
            length = this->yLength();
        if (this->zLength() < length)
            length = this->zLength();
        return length;
    }
    double maxLength() const
    {
        double length = this->xLength();
        if (this->yLength() > length)
            length = this->yLength();
        if (this->zLength() > length)
            length = this->zLength();
        return length;
    }
    /**
     * It is the cube root of the volume.
     */
    double medianLength() const { return this->isAnyInfinite() ?
                    0.0 : std::pow(this->xLength() * this->yLength() * this->zLength(), 1.0 / 3.0); }
    Core::Point3d center() const { return Core::Point3d{this->xMid(), this->yMid(), this->zMid()}; }

    void setXMin(double xMin) { m_xMin = xMin; }
    void setXMax(double xMax) { m_xMax = xMax; }
    void setYMin(double yMin) { m_yMin = yMin; }
    void setYMax(double yMax) { m_yMax = yMax; }
    void setZMin(double zMin) { m_zMin = zMin; }
    void setZMax(double zMax) { m_zMax = zMax; }

    bool isAnyInfinite() const
    {
        return psa::isInfinite(m_xMin) || psa::isInfinite(m_xMax) ||
            psa::isInfinite(m_yMin) || psa::isInfinite(m_yMax) ||
            psa::isInfinite(m_zMin) || psa::isInfinite(m_zMax);
    }
    bool isAllInfinite() const
    {
        return psa::isNegativeInfinite(m_xMin) && psa::isPositiveInfinite(m_xMax) &&
            psa::isNegativeInfinite(m_yMin) && psa::isPositiveInfinite(m_yMax) &&
            psa::isNegativeInfinite(m_zMin) && psa::isPositiveInfinite(m_zMax);
    }

    bool isInside(const Core::Point3d& p) const
    {
        return (m_xMin <= p.x() && p.x() <= m_xMax &&
            m_yMin <= p.y() && p.y() <= m_yMax &&
            m_zMin <= p.z() && p.z() <= m_zMax);
    }

    bool operator==(const Extent& rhs) const
    {
        return psa::isequal(m_xMin, rhs.m_xMin) && psa::isequal(m_xMax, rhs.m_xMax) &&
            psa::isequal(m_yMin, rhs.m_yMin) && psa::isequal(m_yMax, rhs.m_yMax) &&
            psa::isequal(m_zMin, rhs.m_zMin) && psa::isequal(m_zMax, rhs.m_zMax);
    }
    bool operator!=(const Extent& rhs) const { return !(rhs == *this); }

    /**
    * Update the extent with vertex value.
    * Note: As this function needs to be fast, it is inlined. It does not checks for infinite values.
    * It is the caller's resposibility to keep the initial values correct
    * ie <min value> = +<INFINITE> and <max value> = -<INFINITE>.
    */
    template<typename Type>
    void update(const Type* vertex)
    {
        if (vertex[0] < m_xMin) m_xMin = vertex[0];
        if (vertex[1] < m_yMin) m_yMin = vertex[1];
        if (vertex[2] < m_zMin) m_zMin = vertex[2];

        if (vertex[0] > m_xMax) m_xMax = vertex[0];
        if (vertex[1] > m_yMax) m_yMax = vertex[1];
        if (vertex[2] > m_zMax) m_zMax = vertex[2];
    }

    /**
    * Update the extent with vertex value.
    * Note: As this function needs to be fast, it is inlined. It does not checks for infinite values.
    * It is the caller's resposibility to keep the initial values correct
    * ie <min value> = +<INFINITE> and <max value> = -<INFINITE>.
    */
    void update(const Core::Point3d& v)
    {
        if (v.x() < m_xMin) m_xMin = v.x();
        if (v.y() < m_yMin) m_yMin = v.y();
        if (v.z() < m_zMin) m_zMin = v.z();

        if (v.x() > m_xMax) m_xMax = v.x();
        if (v.y() > m_yMax) m_yMax = v.y();
        if (v.z() > m_zMax) m_zMax = v.z();
    }
    void update(const Extent& extent);
    void transform(const Core::Matrix4x4& xformMatrix, CalculationModel model = CalculationModel::Box);
    void transformInfinite(const Core::Matrix4x4& xformMatrix);

private:
    static const int kNumBoxVertices = 8;
    static const int kNumPyramidVertices = 5;

private:
    double m_xMin;
    double m_xMax;
    double m_yMin;
    double m_yMax;
    double m_zMin;
    double m_zMax;
};

} // namespace Geometry

#endif // GEOMETRY_EXTENT_H
