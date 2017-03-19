/*
 * Geometry: Geometry objects for Thanuva
 *
 * Copyright 2016, Saravanan Poosanthiram
 * All rights reserved.
 */

#include "OffMeshReader.h"

#include <fstream>

#include "Mesh.h"
#include "Utils.h"

namespace Geometry {

OffMeshReader::OffMeshReader(const fs::path& filePath)
    : MeshReader{}
    , m_filePath{filePath}
{
}

void OffMeshReader::read(Mesh& mesh)
{
    std::ifstream textStream{m_filePath}; // At this point the file path should be valid. so no error check

    std::string line;
    std::getline(textStream, line); // OFF
    if (Core::trim(line) != "OFF")
        return;

    Core::skipWhitespace(textStream);

    std::regex wsRegex{MeshReader::kWhitespaceRegexPattern};

    std::getline(textStream, line); // numVertices numFaces numEdges
    auto it = std::sregex_token_iterator{line.begin(), line.end(), wsRegex, -1};
    int nVertices = std::stoi(it->str());
    int nFacets = std::stoi((++it)->str());

    mesh.reserve(nVertices * GeometryObject::kValuesPerVertex,
                 nFacets * GeometryObject::kVerticesPerTriangle * GeometryObject::kValuesPerVertex, 0);

    std::array<float, 3> vertex;
    while (nVertices-- > 0) {
        std::getline(textStream, line);
        it = std::sregex_token_iterator{line.begin(), line.end(), wsRegex, -1};
        this->readVertex(it, vertex);
        mesh.insertVertex(&vertex[0]);
    }

    std::array<int, 3> indices;
    while (nFacets-- > 0) {
        std::getline(textStream, line);
        it = std::sregex_token_iterator{line.begin(), line.end(), wsRegex, -1};
        this->readIndices(++it, indices);
        if (!this->isAllNormalAvailable(mesh, indices)) {
            Core::Vector3d normal = mesh.computeNormal(indices[0], indices[1], indices[2]);
            std::array<int, 3> sortedIndices = indices;
            std::sort(sortedIndices.begin(), sortedIndices.end());
            for (auto index : sortedIndices) {
                if (index * GeometryObject::kValuesPerVertex == mesh.normals().size())
                    mesh.insertNormal(normal);
            }
        }
        mesh.insertIndices(indices[0], indices[1], indices[2]);
    }
}

void OffMeshReader::readVertex(std::sregex_token_iterator it, std::array<float, 3>& vertex)
{
    const std::sregex_token_iterator endIt{};
    for (int i = 0; i < vertex.size() && it != endIt; ++i, ++it)
        vertex[i] = std::stof(it->str());
}

void OffMeshReader::readIndices(std::sregex_token_iterator it, std::array<int, 3>& indices)
{
    const std::sregex_token_iterator endIt{};
    for (int i = 0; i < indices.size() && it != endIt; ++i, ++it)
        indices[i] = std::stoi(it->str());
}

bool OffMeshReader::isAllNormalAvailable(Mesh& mesh, const std::array<int, 3>& indices)
{
    auto& normals = mesh.normals();
    for (auto index : indices) {
        if (index * GeometryObject::kValuesPerVertex >= normals.size())
            return false;
    }

    return true;
}

} // namespace Geometry
