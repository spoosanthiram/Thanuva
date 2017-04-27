/*
 * Geometry: Geometry objects for Thanuva
 *
 * Copyright 2016, Saravanan Poosanthiram
 * All rights reserved.
 */

#ifndef Geometry_Sphere_h
#define Geometry_Sphere_h

#include <deque>
#include <map>

#include "Geometry.h"

namespace Model { class SphereModel; }

namespace Geometry {

class Sphere : public Geometry
{
public:
    Sphere(const GeometryScene* geometryScene, Model::SphereModel* sphereModel);

private: // slots
    void initialize();

private:
    void initializeOctahedron(std::deque<int>& indices);
    void subdivide(std::deque<int>& indices);
    int midVertexIndex(int i0, int i1, std::map<int64_t, int>&midVertexIndexMap);
};

} // namespace Geometry

#endif // Geometry_Sphere_h
