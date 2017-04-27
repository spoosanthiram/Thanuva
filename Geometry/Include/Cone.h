/*
 * Geometry: Geometry objects for Thanuva
 *
 * Copyright 2016, Saravanan Poosanthiram
 * All rights reserved.
 */

#ifndef Geometry_Cone_h
#define Geometry_Cone_h

#include "Geometry.h"

namespace Model { class ConeModel; }

namespace Geometry {

class Cone : public Geometry
{
public:
    Cone(const GeometryScene* geometryScene, Model::ConeModel* coneModel);

private: // slots
    void initialize();

private:
    void generateBaseTriangles(const std::vector<Core::Vector3d>& initVectors, const Core::Vector3d& normal);
    void generateSideTriangles(const std::vector<Core::Vector3d>& initVectors);
};

} // namespace Geometry

#endif // Geometry_Cone_h
