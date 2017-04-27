/*
 * Geometry: Geometry objects for Thanuva
 *
 * Copyright 2016, Saravanan Poosanthiram
 * All rights reserved.
 */

#ifndef Geometry_CoordinateSystem_h
#define Geometry_CoordinateSystem_h

#include "Matrix4x4.h"
#include "ThanuvaGeometry.h"

namespace Model { class CoordinateSystemModel; }

namespace Geometry {

class CoordinateSystem : public ThanuvaGeometry {
public:
    CoordinateSystem(const GeometryScene* geometryScene, Model::CoordinateSystemModel* csysModel);

    const Core::Matrix4x4& transformMatrix() const { return m_transformMatrix; }

private:
    Core::Matrix4x4 m_transformMatrix;
};

} // namespace Geometry

#endif //Geometry_CoordinateSystem_h
