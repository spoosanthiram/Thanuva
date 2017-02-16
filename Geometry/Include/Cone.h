/*
 * Geometry: Geometry objects for Thanuva
 *
 * Copyright 2016, Saravanan Poosanthiram
 * All rights reserved.
 */

#ifndef GEOMETRY_CONE_H
#define GEOMETRY_CONE_H

#include "GeometryObject.h"

namespace Model { class ConeModel; }

namespace Geometry {

class Cone : public GeometryObject
{
public:
    Cone(const Core::Point3d& apex, const Core::Point3d& center, double radius, unsigned int numFacets)
        : GeometryObject{nullptr, nullptr}
    {
        this->initialize(apex, center, radius, numFacets);
    }

private: // slots
    void initialize();

private:
    void initialize(const Core::Point3d& apex, const Core::Point3d& center,
                    double radius, unsigned int numFacets);
    void generateBaseTriangles(const Core::Point3d& center, double radius, unsigned int numFacets,
                               const std::vector<Core::Vector3d>& initVectors, const Core::Vector3d& normal);
    void generateSideTriangles(const Core::Point3d& apex, unsigned int numFacets,
                               const std::vector<Core::Vector3d>& initVectors);
};

} // namespace Geometry

#endif // GEOMETRY_CONE_H
