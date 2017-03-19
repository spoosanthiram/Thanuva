/*
 * Geometry: Geometry objects for Thanuva
 *
 * Copyright 2016, Saravanan Poosanthiram
 * All rights reserved.
 */

#ifndef GEOMETRY_MESHREADER_H
#define GEOMETRY_MESHREADER_H

#include <filesystem>
#include <memory>
#include <string>

namespace fs = std::experimental::filesystem;

namespace Geometry {

class Mesh;

class MeshReader
{
public:
    static std::unique_ptr<MeshReader> getInstance(const fs::path& filePath);

    static const char* kWhitespaceRegexPattern;

public:
    MeshReader() {}
    virtual ~MeshReader() {}

    virtual void read(Mesh& mesh) = 0;
};

} // namespace Geometry

#endif // GEOMETRY_MESHREADER_H
