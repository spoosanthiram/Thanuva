/*
 * Geometry: Geometry objects for Thanuva
 *
 * Copyright 2016, Saravanan Poosanthiram
 * All rights reserved.
 */

#ifndef Geometry_Cylinder_h
#define Geometry_Cylinder_h

#include "Geometry.h"

namespace Model { class CylinderModel; }

namespace Geometry {

class Cylinder : public Geometry
{
public:
    Cylinder(const SceneGeometry* sceneGeometry, Model::CylinderModel* cylinderModel);

private: // slots
    void initialize();

private:
    void generateFace1Triangles(const std::vector<Core::Vector3d>& initVectors, const Core::Vector3d& normal);
    void generateFace2Triangles(const std::vector<Core::Vector3d>& initVectors, const Core::Vector3d& normal);
    void generateFaceVertices(const Core::Point3d& endpoint, double radius,
                              const std::vector<Core::Vector3d>& initVectors, const Core::Vector3d& normal);
    void generateSideTriangles(const std::vector<Core::Vector3d>& initVectors);
};

} // namespace Geometry

#endif // Geometry_Cylinder_h
