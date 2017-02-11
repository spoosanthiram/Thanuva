/*
 * Geometry: Geometry objects for Thanuva
 *
 * Copyright 2016, Saravanan Poosanthiram
 * All rights reserved.
 */

#ifndef GEOMETRY_STL_H
#define GEOMETRY_STL_H

#include <regex>
#include <unordered_map>

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
    Stl(const GeometryContainer& gfxProject, Model::StlModel* stlModel);

private: // slots
    void initialize();

private:
    bool isAsciiFormat(const std::string& filePath) const;
    void readAscii(const std::string& filePath);
    void readValues(std::sregex_token_iterator it, float* values)
    {
        const std::sregex_token_iterator endIt{};
        int i = 0;
        for (++it; it != endIt; ++it)
            values[i++] = std::stof(it->str());
    }
    void readBinary(const std::string& filePath);
    void processData(char* p, char* q, VertexIndexMapType& vertexIndexMap);

    /**
     * Get the index either one already there or newly inserted one.
     */
    int index(float* vertex, float* normal, VertexIndexMapType& vertexIndexMap)
    {
        Vertex v{vertex};
        auto it = vertexIndexMap.find(v);
        if (it != vertexIndexMap.end()) {
            const float* currentNormal = &(this->normals()[it->second * kValuesPerVertex]);
            if (currentNormal[0] == normal[0] && currentNormal[1] == normal[1] && currentNormal[2] == normal[2])
                return it->second;
        }

        int index = static_cast<int>(this->vertices().size() / kValuesPerVertex);
        vertexIndexMap[v] = index;

        this->insertVertex(v.values);
        this->insertNormal(normal);

        return index;
    }
};

} // namespace Geometry

#endif // GEOMETRY_STL_H
