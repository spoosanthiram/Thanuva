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
    Cylinder(const GeometryContainer& geometryContainer, Model::CylinderModel* cylinderModel);

private: // slots
    void initialize();

private:
    std::vector<Core::Vector3d> genInitialVectors(unsigned int numFacets,
                                                  const Core::Vector3d& u, const Core::Vector3d& v);
    void generateFace1Triangles(const std::vector<Core::Vector3d>& initVectors, const Core::Vector3d& normal);
    void generateFace2Triangles(const std::vector<Core::Vector3d>& initVectors, const Core::Vector3d& normal);
    void generateFaceVertices(const Core::Point3d& endpoint, double radius,
                              const std::vector<Core::Vector3d>& initVectors, const Core::Vector3d& normal);
    void generateSideTriangles(const std::vector<Core::Vector3d>& initVectors);
};

} // namespace Geometry

#endif // GEOMETRY_CYLINDER_H
