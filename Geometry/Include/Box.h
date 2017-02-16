/*
 * Geometry: Geometry objects for Thanuva
 *
 * Copyright 2016, Saravanan Poosanthiram
 * All rights reserved.
 */

#ifndef GEOMETRY_BOX_H
#define GEOMETRY_BOX_H

#include "GeometryObject.h"

namespace Model { class BoxModel; }

namespace Geometry {

class Box : public GeometryObject
{
public:
    Box(const GeometryContainer* geometryContainer, Model::BoxModel* boxModel);

private: // slots
    void initialize();

private:
    void insertQuad(const Core::Point3d& a, const Core::Point3d& b,
                    const Core::Point3d& c, const Core::Point3d& d);
};

} // namespace Geometry

#endif // GEOMETRY_BOX_H
