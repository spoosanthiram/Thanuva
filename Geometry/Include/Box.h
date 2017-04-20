/*
 * Geometry: Geometry objects for Thanuva
 *
 * Copyright 2016, Saravanan Poosanthiram
 * All rights reserved.
 */

#ifndef Geometry_Box_h
#define Geometry_Box_h

#include "Geometry.h"

namespace Model { class BoxModel; }

namespace Geometry {

class Box : public Geometry
{
public:
    Box(const SceneGeometry* sceneGeometry, Model::BoxModel* boxModel);

private: // slots
    void initialize();

private:
    void insertQuad(const Core::Point3d& a, const Core::Point3d& b,
                    const Core::Point3d& c, const Core::Point3d& d);
};

} // namespace Geometry

#endif // Geometry_Box_h
