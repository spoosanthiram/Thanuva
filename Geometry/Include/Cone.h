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
    Cone(const GeometryContainer* geometryContainer, Model::ConeModel* coneModel);

private: // slots
    void initialize();

private:
    void generateBaseTriangles(const std::vector<Core::Vector3d>& initVectors, const Core::Vector3d& normal);
    void generateSideTriangles(const std::vector<Core::Vector3d>& initVectors);
};

} // namespace Geometry

#endif // GEOMETRY_CONE_H
