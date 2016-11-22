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
    Box(const GeometryContainer& geometryContainer, Model::BoxModel* boxModel);

    bool intersect(const Core::Vector3d& nearPoint, const Core::Vector3d& farPoint,
                   std::vector<Core::Vector3d>* points) override;

private: // slots
    void initialize();

private:
    void insertQuad(const Core::Vector3d& a, const Core::Vector3d& b,
                    const Core::Vector3d& c, const Core::Vector3d& d);
};

} // namespace Geometry

#endif // GEOMETRY_BOX_H
