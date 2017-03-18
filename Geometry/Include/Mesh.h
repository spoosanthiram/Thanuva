/*
 * Geometry: Geometry objects for Thanuva
 *
 * Copyright 2016, Saravanan Poosanthiram
 * All rights reserved.
 */

#ifndef GEOMETRY_MESH_H
#define GEOMETRY_MESH_H

#include <regex>
#include <unordered_map>

#include "GeometryObject.h"

namespace Model { class MeshModel; }

namespace Geometry {

class Mesh : public GeometryObject
{
public:
    Mesh(const GeometryContainer* geometryContainer, Model::MeshModel* meshModel);

private: // slots
    void initialize();
};

} // namespace Geometry

#endif // GEOMETRY_MESH_H
