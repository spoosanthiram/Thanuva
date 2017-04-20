/*
 * Geometry: Geometry objects for Thanuva
 *
 * Copyright 2016, Saravanan Poosanthiram
 * All rights reserved.
 */

#include "StlMeshReader.h"

#include <fstream>

#include <glog/logging.h>

#include "Utils.h"

namespace Geometry {

StlMeshReader::StlMeshReader(const fs::path& filePath)
    : MeshReader{}
    , m_filePath{filePath}
{
}

void StlMeshReader::read(Mesh& mesh)
{
    if (this->isAsciiFormat())
        this->readAscii(mesh);
    else
        this->readBinary(mesh);
}

bool StlMeshReader::isAsciiFormat() const
{
    const int kBufLen = 256;

    std::ifstream stlInputStream{m_filePath};
    CHECK(stlInputStream);

    std::string buf{};
    buf.resize(kBufLen, '\0');
    stlInputStream.read(&buf[0], kBufLen-1);

    return (buf.find("facet") != std::string::npos || buf.find("normal") != std::string::npos) ? true : false;
}

void StlMeshReader::readAscii(Mesh& mesh)
{
    std::ifstream textStream{m_filePath}; // At this point the file path should be valid. so no error check
    Core::skipWhitespace(textStream);

    std::string line;
    std::getline(textStream, line); // "solid <name>"

    VertexIndexMapType vertexIndexMap;
    float normal[3];
    float vertex[9];
    int indices[3];
    std::regex spacePattern{Core::kWhitespaceRegEx};

    while (textStream) {
        Core::skipWhitespace(textStream);
        std::getline(textStream, line);

        if (line.find("endsolid") != std::string::npos)
            break;

        auto it = std::sregex_token_iterator{line.begin(), line.end(), spacePattern, -1};
        if (it->str() == "facet") { // "facet normal <number> <number> <number>"
            this->readValues(++it, 3, normal);
            Core::skipWhitespace(textStream);

            std::getline(textStream, line);
            if (line.find("outer") != std::string::npos) { // "outer loop"
                int nvertex = 0;
                while (textStream && nvertex < 3) {
                    Core::skipWhitespace(textStream);
                    std::getline(textStream, line);
                    it = std::sregex_token_iterator{line.begin(), line.end(), spacePattern, -1};
                    if (it->str() == "vertex") // vertex <number> <number> <number>
                        this->readValues(it, 3, &vertex[nvertex * 3]);
                    indices[nvertex] = this->index(mesh, &vertex[nvertex * 3], normal, vertexIndexMap);
                    ++nvertex;
                }

                Core::skipWhitespace(textStream);
                std::getline(textStream, line); // "endloop"
            }

            Core::skipWhitespace(textStream);
            std::getline(textStream, line); // "endfacet"

            mesh.insertIndices(indices[0], indices[1], indices[2]);
        }
    }
}

void StlMeshReader::readValues(std::sregex_token_iterator it, int nValues, float* values)
{
    const std::sregex_token_iterator endIt{};
    ++it;
    for (int i = 0; i < nValues && it != endIt; i++, ++it)
        values[i] = std::stof(it->str());
}

void StlMeshReader::readBinary(Mesh& mesh)
{
    std::ifstream dataStream{m_filePath, std::ios::binary}; // At this point the file path should be valid. so no error check

    auto dataStreamRdBuf = std::make_unique<char[]>(kDataStreamReadBufferSize);
    dataStream.rdbuf()->pubsetbuf(dataStreamRdBuf.get(), kDataStreamReadBufferSize);

    auto buf = std::make_unique<char[]>(kNFacetChunk * kFacetSize);
    dataStream.read(buf.get(), kBinaryHeaderLength); // read STL header

    dataStream.read(buf.get(), sizeof(int)); // read number of facets
    unsigned int nFacets = *reinterpret_cast<int*>(buf.get());

    mesh.reserve(nFacets * Geometry::kVerticesPerTriangle * Geometry::kValuesPerVertex,
                 nFacets * Geometry::kVerticesPerTriangle * Geometry::kValuesPerVertex, 0);

    VertexIndexMapType vertexIndexMap{nFacets * 2};
    int readSize = kNFacetChunk;

    for (unsigned int i = 0; i < nFacets; i += readSize) {
        readSize = ((nFacets - i) > kNFacetChunk) ? kNFacetChunk : (nFacets - i);
        dataStream.read(buf.get(), readSize * kFacetSize);
        this->processData(mesh, buf.get(), buf.get() + (readSize * kFacetSize), vertexIndexMap);
    }
}

void StlMeshReader::processData(Mesh& mesh, char* p, char* q, VertexIndexMapType& vertexIndexMap)
{
    while (p < q) {
        float* normal = reinterpret_cast<float*>(p);
        p += 3 * sizeof(float);

        int i1 = this->index(mesh, reinterpret_cast<float*>(p), normal, vertexIndexMap);
        p += 3 * sizeof(float);

        int i2 = this->index(mesh, reinterpret_cast<float*>(p), normal, vertexIndexMap);
        p += 3 * sizeof(float);

        int i3 = this->index(mesh, reinterpret_cast<float*>(p), normal, vertexIndexMap);
        p += 3 * sizeof(float);

        p += sizeof(uint16_t);

        mesh.insertIndices(i1, i2, i3);
    }
}

} // namespace Geometry
