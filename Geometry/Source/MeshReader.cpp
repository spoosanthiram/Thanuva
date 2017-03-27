/*
* Geometry: Geometry objects for Thanuva
*
* Copyright 2016, Saravanan Poosanthiram
* All rights reserved.
*/

#include "MeshReader.h"

#include "OffMeshReader.h"
#include "StlMeshReader.h"

namespace Geometry {

std::unique_ptr<MeshReader> MeshReader::getInstance(const fs::path& filePath)
{
    fs::path ext = filePath.extension();
    if (ext == ".stl")
        return std::make_unique<StlMeshReader>(filePath);
    else if (ext == ".off")
        return std::make_unique<OffMeshReader>(filePath);
    else
        return std::unique_ptr<MeshReader>{};
}

} // namespace Geometry
