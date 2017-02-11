/*
 * Geometry: Geometry objects for Thanuva
 *
 * Copyright 2016, Saravanan Poosanthiram
 * All rights reserved.
 */

#include "Stl.h"

#include <fstream>
#include <thread>

#include <glog/logging.h>

#include "StlModel.h"
#include "Utils.h"

namespace Geometry {

Stl::Stl(const GeometryContainer& geometryContainer, Model::StlModel* stlModel)
    : GeometryObject(geometryContainer, stlModel)
{
    this->initialize();

    stlModel->filePathChanged.connect<Stl, &Stl::initialize>(this);
}

void Stl::initialize()
{
    this->clear();

    this->setExtent(Extent{ std::numeric_limits<double>::infinity(), -std::numeric_limits<double>::infinity(),
                    std::numeric_limits<double>::infinity(), -std::numeric_limits<double>::infinity(),
                    std::numeric_limits<double>::infinity(), -std::numeric_limits<double>::infinity() });

    std::string filePath = static_cast<Model::StlModel*>(this->modelObject())->filePath();
    if (this->isAsciiFormat(filePath))
        this->readAscii(filePath);
    else
        this->readBinary(filePath);

    this->initializeBoundingBox();

    // emit signals
    geometryObjectChanged.emit_signal();
    extentChanged.emit_signal();
}

bool Stl::isAsciiFormat(const std::string& filePath) const
{
    const int kBufLen = 256;

    std::ifstream stlInputStream{filePath};
    CHECK(stlInputStream) << "STL file stream";

    std::string buf{};
    buf.resize(kBufLen, '\0');
    stlInputStream.read(&buf[0], kBufLen-1);

    return (buf.find("facet") != std::string::npos || buf.find("normal") != std::string::npos) ? true : false;
}

void Stl::readAscii(const std::string& filePath)
{
    std::ifstream textStream{filePath}; // At this point the file path should be valid. so no error check
    Core::skipWhitespace(textStream);

    std::string line;
    std::getline(textStream, line); // "solid <name>"

    VertexIndexMapType vertexIndexMap;
    float normal[3];
    float vertex[9];
    int indices[3];
    std::regex spacePattern{ R"(\s+)" };

    while (textStream) {
        Core::skipWhitespace(textStream);
        std::getline(textStream, line);

        if (line.find("endsolid") != std::string::npos)
            break;

        auto it = std::sregex_token_iterator{line.begin(), line.end(), spacePattern, -1};
        if (it->str() == "facet") { // "facet normal <number> <number> <number>"
            this->readValues(++it, normal);
            Core::skipWhitespace(textStream);

            std::getline(textStream, line);
            if (line.find("outer") != std::string::npos) { // "outer loop"
                int nvertex = 0;
                while (textStream && nvertex < 3) {
                    Core::skipWhitespace(textStream);
                    std::getline(textStream, line);
                    it = std::sregex_token_iterator{line.begin(), line.end(), spacePattern, -1};
                    if (it->str() == "vertex") // vertex <number> <number> <number>
                        this->readValues(it, &vertex[nvertex * 3]);
                    indices[nvertex] = this->index(&vertex[nvertex * 3], normal, vertexIndexMap);
                    ++nvertex;
                }

                Core::skipWhitespace(textStream);
                std::getline(textStream, line); // "endloop"
            }

            Core::skipWhitespace(textStream);
            std::getline(textStream, line); // "endfacet"

            this->insertIndices(indices[0], indices[1], indices[2]);
        }
    }
}

void Stl::readBinary(const std::string& filePath)
{
    std::ifstream dataStream{filePath, std::ios::binary}; // At this point the file path should be valid. so no error check

    auto dataStreamRdBuf = std::make_unique<char[]>(kDataStreamReadBufferSize);
    dataStream.rdbuf()->pubsetbuf(dataStreamRdBuf.get(), kDataStreamReadBufferSize);

    auto buf = std::make_unique<char[]>(kNFacetChunk * kFacetSize);
    dataStream.read(buf.get(), kBinaryHeaderLength); // read STL header

    dataStream.read(buf.get(), sizeof(int)); // read number of facets
    unsigned int nFacets = *reinterpret_cast<int*>(buf.get());

    this->reserve(nFacets * GeometryObject::kVerticesPerTriangle * GeometryObject::kValuesPerVertex,
                  nFacets * GeometryObject::kVerticesPerTriangle * GeometryObject::kValuesPerVertex, 0);

    VertexIndexMapType vertexIndexMap{nFacets * 2};
    int readSize = kNFacetChunk;

    for (unsigned int i = 0; i < nFacets; i += readSize) {
        readSize = ((nFacets - i) > kNFacetChunk) ? kNFacetChunk : (nFacets - i);
        dataStream.read(buf.get(), readSize * kFacetSize);
        this->processData(buf.get(), buf.get() + (readSize * kFacetSize), vertexIndexMap);
    }
}

void Stl::processData(char* p, char* q, VertexIndexMapType& vertexIndexMap)
{
    while (p < q) {
        float* normal = reinterpret_cast<float*>(p);
        p += 3 * sizeof(float);

        int i1 = this->index(reinterpret_cast<float*>(p), normal, vertexIndexMap);
        p += 3 * sizeof(float);

        int i2 = this->index(reinterpret_cast<float*>(p), normal, vertexIndexMap);
        p += 3 * sizeof(float);

        int i3 = this->index(reinterpret_cast<float*>(p), normal, vertexIndexMap);
        p += 3 * sizeof(float);

        p += sizeof(uint16_t);

        this->insertIndices(i1, i2, i3);
    }
}

} // namespace Geometry
