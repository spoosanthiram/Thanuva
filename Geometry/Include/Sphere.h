/*
 * Geometry: Geometry objects for Thanuva
 *
 * Copyright 2016, Saravanan Poosanthiram
 * All rights reserved.
 */

#ifndef GEOMETRY_SPHERE_H
#define GEOMETRY_SPHERE_H

#include "GeometryObject.h"

namespace Model { class SphereModel; }

namespace Geometry {

class Sphere : public GeometryObject
{
public:
    Sphere(const GeometryContainer* geometryContainer, Model::SphereModel* sphereModel);

private: // slots
    void initialize();

private:
    void initializeOctahedron();
};

} // namespace Geometry

#endif // GEOMETRY_SPHERE_H
