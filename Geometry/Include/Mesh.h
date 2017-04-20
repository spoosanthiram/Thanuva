/*
 * Geometry: Geometry objects for Thanuva
 *
 * Copyright 2016, Saravanan Poosanthiram
 * All rights reserved.
 */

#ifndef Geometry_Meesh_h
#define Geometry_Meesh_h

#include "Geometry.h"

namespace Model { class MeshModel; }

namespace Geometry {

class Mesh : public Geometry
{
public:
    Mesh(const SceneGeometry* sceneGeometry, Model::MeshModel* meshModel);

private: // slots
    void initialize();
};

} // namespace Geometry

#endif // Geometry_Meesh_h
