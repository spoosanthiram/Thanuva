/*
 * Geometry: Geometry objects for Thanuva
 *
 * Copyright 2016, Saravanan Poosanthiram
 * All rights reserved.
 */

#ifndef GEOMETRY_CYLINDER_H
#define GEOMETRY_CYLINDER_H

#include "GeometryObject.h"

namespace Model { class CylinderModel; }

namespace Geometry {

class Cylinder : public GeometryObject
{
public:
    Cylinder(const GeometryContainer* geometryContainer, Model::CylinderModel* cylinderModel);
    Cylinder(const Core::Point3d& endpoint1, double radius1, const Core::Point3d& endpoint2, double radius2,
             unsigned int numFacets)
        : GeometryObject{nullptr, nullptr}
    {
        this->initialize(endpoint1, radius1, endpoint2, radius2, numFacets);
    }

private: // slots
    void initialize();

private:
    void initialize(const Core::Point3d& endpoint1, double radius1,
                    const Core::Point3d& endpoint2, double radius2,
                    unsigned int numFacets);
    void generateFace1Triangles(const Core::Point3d& endpoint1, double radius1, unsigned int numFacets,
                                const std::vector<Core::Vector3d>& initVectors, const Core::Vector3d& normal);
    void generateFace2Triangles(const Core::Point3d& endpoint2, double radius2, unsigned int numFacets,
                                const std::vector<Core::Vector3d>& initVectors, const Core::Vector3d& normal);
    void generateFaceVertices(const Core::Point3d& endpoint, double radius,
                              const std::vector<Core::Vector3d>& initVectors, const Core::Vector3d& normal);
    void generateSideTriangles(unsigned int numFacets, const std::vector<Core::Vector3d>& initVectors);
};

} // namespace Geometry

#endif // GEOMETRY_CYLINDER_H
