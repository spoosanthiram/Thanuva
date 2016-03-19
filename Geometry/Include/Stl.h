/*
 * Geometry: Geometry objects for Thanuva
 *
 * Copyright 2016, Saravanan Poosanthiram
 * All rights reserved.
 */

#ifndef GEOMETRY_STL_H
#define GEOMETRY_STL_H

#include "GeometryObject.h"

namespace Model { class StlModel; }

namespace Geometry {

class Stl : public GeometryObject
{
public:
    static const int kBinaryHeaderLength = 80;
    static const int kDataStreamReadBufferSize = 2097152;
    static const int kFacetSize = 12 * sizeof(float) + sizeof(uint16_t); // 1 normal and three vertices = 12 floats; uint16_t for pad
    static const int kNFacetChunk = 10000;

public:
    Stl(const GeometryContainer& gfxProject, Model::StlModel* stlModel);

    bool intersect(const Core::Vector3d& nearPoint, const Core::Vector3d& farPoint, std::vector<Core::Vector3d>* points) override;

protected: // slots
    void initialize();
};

} // namespace Geometry

#endif // GEOMETRY_STL_H
