/*
* Geometry: Geometry objects for Thanuva
*
* Copyright 2016, Saravanan Poosanthiram
* All rights reserved.
*/

#ifndef GEOMETRY_OFFMESHREADER_H
#define GEOMETRY_OFFMESHREADER_H

#include <array>
#include <regex>

#include "MeshReader.h"

namespace Geometry {

class OffMeshReader : public MeshReader
{
public:
    OffMeshReader(const fs::path& filePath);

    void read(Mesh& mesh) override;

private:
    void readVertex(std::sregex_token_iterator it, std::array<float, 3>& vertex);
    void readIndices(std::sregex_token_iterator it, std::array<int, 3>& indices);
    bool isAllNormalAvailable(Mesh& mesh, const std::array<int, 3>& indices);

    fs::path m_filePath;
};


} // namespace Geometry

#endif // GEOMETRY_OFFMESHREADER_H
