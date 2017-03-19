/*
 * Geometry: Geometry objects for Thanuva
 *
 * Copyright 2016, Saravanan Poosanthiram
 * All rights reserved.
 */

#ifndef GEOMETRY_STLMESHREADER_H
#define GEOMETRY_STLMESHREADER_H

#include <regex>
#include <unordered_map>

#include <AlgoBase.h>

#include "Mesh.h"
#include "MeshReader.h"

namespace Geometry {

class StlMeshReader : public MeshReader
{
public:
    static const int kBinaryHeaderLength = 80;
    static const int kDataStreamReadBufferSize = 2097152;
    static const int kFacetSize = 12 * sizeof(float) + sizeof(uint16_t); // 1 normal and three vertices = 12 floats; uint16_t for pad
    static const int kNFacetChunk = 10000;

    struct Vertex
    {
        Vertex(float* v)
        {
            values[0] = v[0];
            values[1] = v[1];
            values[2] = v[2];
        }
        bool operator== (const Vertex& rhs) const
        {
            return psa::isequal(values[0], rhs.values[0]) &&
                   psa::isequal(values[1], rhs.values[1]) &&
                   psa::isequal(values[2], rhs.values[2]);
        }
        float values[3];
    };

    struct VertexHasher
    {
        std::size_t combine(size_t hashVal, float x) const
        {
            return hashVal ^ (std::hash<float>{}(x) + 0x9e3779b9 + (hashVal << 6) + (hashVal >> 2));
        }

        std::size_t operator()(const Vertex& p) const
        {
            std::size_t hashVal = 0;
            hashVal = combine(hashVal, p.values[0]);
            hashVal = combine(hashVal, p.values[1]);
            hashVal = combine(hashVal, p.values[2]);
            return hashVal;
        }
    };

    using VertexIndexMapType = std::unordered_map<Vertex, int, VertexHasher>;

public:
    StlMeshReader(const fs::path& filePath);

    void read(Mesh& mesh) override;

private:
    bool isAsciiFormat() const;
    void readAscii(Mesh& mesh);
    void readValues(std::sregex_token_iterator it, int nValues, float* values);
    void readBinary(Mesh& mesh);
    void processData(Mesh& mesh, char* p, char* q, VertexIndexMapType& vertexIndexMap);

    /**
     * Get the index either one already there or newly inserted one.
     */
    int index(Mesh& mesh, float* vertex, float* normal, VertexIndexMapType& vertexIndexMap)
    {
        Vertex v{vertex};
        auto it = vertexIndexMap.find(v);
        if (it != vertexIndexMap.end()) {
            const float* currentNormal = &(mesh.normals()[it->second * GeometryObject::kValuesPerVertex]);
            if (currentNormal[0] == normal[0] && currentNormal[1] == normal[1] && currentNormal[2] == normal[2])
                return it->second;
        }

        int index = static_cast<int>(mesh.vertices().size() / GeometryObject::kValuesPerVertex);
        vertexIndexMap[v] = index;

        mesh.insertVertex(v.values);
        mesh.insertNormal(normal);

        return index;
    }

private:
    fs::path m_filePath;
};

} // namespace Geometry

#endif // GEOMETRY_STLMESHREADER_H
