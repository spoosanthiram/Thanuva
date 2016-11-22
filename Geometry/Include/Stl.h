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

    struct Vertex
    {
        void assign(float* v)
        {
            values[0] = v[0];
            values[1] = v[1];
            values[2] = v[2];
        }
        bool operator= (const Vertex& rhs)
        {
            return psa::isequal(values[0], rhs.values[0]) &&
                   psa::isequal(values[1], rhs.values[1]) &&
                   psa::isequal(values[2], rhs.values[2]);
        }
        float values[3];
    };

    struct VertexHasher
    {
        std::size_t combine(size_t hashVal, float x)
        {
            return hashVal ^ (std::hash<float>{}(x) + 0x9e3779b9 + (hashVal << 6) + (hashVal >> 2));
        }

        std::size_t operator()(const Vertex& p)
        {
            std::size_t hashVal = 0;
            hashVal = combine(hashVal, p.x);
            hashVal = combine(hashVal, p.y);
            hashVal = combine(hashVal, p.z);
            return hashVal;
        }
    };

public:
    Stl(const GeometryContainer& gfxProject, Model::StlModel* stlModel);

    bool intersect(const Core::Vector3d& nearPoint, const Core::Vector3d& farPoint,
                   std::vector<Core::Vector3d>* points) override;

protected: // slots
    void initialize();
};

} // namespace Geometry

#endif // GEOMETRY_STL_H
