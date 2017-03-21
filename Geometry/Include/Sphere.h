/*
 * Geometry: Geometry objects for Thanuva
 *
 * Copyright 2016, Saravanan Poosanthiram
 * All rights reserved.
 */

#ifndef GEOMETRY_SPHERE_H
#define GEOMETRY_SPHERE_H

#include <deque>
#include <map>

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
    void initializeOctahedron(std::deque<int>& indices);
    void subdivide(std::deque<int>& indices);
    int midVertexIndex(int i0, int i1, std::map<int64_t, int>&midVertexIndexMap);
};

} // namespace Geometry

#endif // GEOMETRY_SPHERE_H
