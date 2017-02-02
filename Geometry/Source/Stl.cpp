/*
 * Geometry: Geometry objects for Thanuva
 *
 * Copyright 2016, Saravanan Poosanthiram
 * All rights reserved.
 */

#include "Stl.h"

#include <chrono>
#include <fstream>
#include <thread>

#include <glog/logging.h>

#include "StlModel.h"
#include "Utils.h"

namespace Geometry {

const std::size_t k2MillionTriangles = 2000000 * 9;
const std::size_t k4Point5MillionTriangles = 4500000 * 9;

Stl::Stl(const GeometryContainer& geometryContainer, Model::StlModel* stlModel)
    : GeometryObject(geometryContainer, stlModel)
{
    this->initialize();

    stlModel->filePathChanged.connect<Stl, &Stl::initialize>(this);
}

struct IntersectionPoints {
    IntersectionPoints(const std::vector<float>& vertices, const std::vector<float>& normals,
                       std::size_t istart, std::size_t iend,
                       const Core::Point3d& nearPoint, const Core::Point3d& farPoint,
                       volatile bool& found, std::vector<Core::Point3d>* points)
        : m_vertices{vertices}
        , m_normals{normals}
        , m_istart{istart}
        , m_iend{iend}
        , m_nearPoint{nearPoint}
        , m_farPoint{farPoint}
        , m_found{found}
        , m_points{points}
    {}

    void operator()() {
        Core::Vector3d n;
        Core::Point3d a, b, c, p;
        Core::Vector3d l = m_farPoint - m_nearPoint;

        for (size_t i = m_istart; i < m_iend; i += 9) {
            if (m_found && !m_points)
                break;

            a.assign(&m_vertices[i]);
            n.assign(&m_normals[i]);

            if (!GeometryObject::intersectPlane(a, n, m_nearPoint, l, p))
                continue;

            b.assign(&m_vertices[i + 3]);
            c.assign(&m_vertices[i + 6]);

            // is the point p inside the triangle?
            if (n.dot((b - a).cross(p - a)) >= 0.0 &&
                n.dot((c - b).cross(p - b)) >= 0.0 &&
                n.dot((a - c).cross(p - c)) >= 0.0)
            {
                m_found = true;
                if (m_points)
                    m_points->push_back(p);
                else
                    break;
            }
        }
    }

    const std::vector<float>& m_vertices;
    const std::vector<float>& m_normals;
    std::size_t m_istart;
    std::size_t m_iend;
    const Core::Point3d& m_nearPoint;
    const Core::Point3d& m_farPoint;
    volatile bool& m_found;
    std::vector<Core::Point3d>* m_points;
};

bool Stl::intersect(const Core::Point3d& nearPoint, const Core::Point3d& farPoint,
                    std::vector<Core::Point3d>* points)
{
    const auto& vertices = this->vertices();
    const auto& normals = this->normals();
    if (vertices.size() <= 0 || vertices.size() != normals.size() || (vertices.size() % 9) != 0)
        return false;

    volatile bool found = false;

    auto time1 = std::chrono::steady_clock::now();

    std::size_t size = vertices.size();
    if (size <= k2MillionTriangles)
        IntersectionPoints{vertices, normals, 0, size, nearPoint, farPoint, found, points}();
    else if (size <= k4Point5MillionTriangles) {
        std::size_t size1 = size / 2;
        std::thread thread1{IntersectionPoints{vertices, normals, 0, size1,
                        nearPoint, farPoint, found, points}};
        IntersectionPoints{vertices, normals, size1, size, nearPoint, farPoint, found, points}();
        thread1.join();
    }
    else {
        std::size_t sizeInc = size / 4;

        std::size_t istart = 0;
        std::size_t iend = sizeInc;
        std::thread thread1{IntersectionPoints{vertices, normals, istart, iend,
                        nearPoint, farPoint, found, points}};

        istart = iend;
        iend = istart + sizeInc;
        std::thread thread2{IntersectionPoints{vertices, normals, istart, iend,
                        nearPoint, farPoint, found, points}};

        istart = iend;
        iend = istart + sizeInc;
        std::thread thread3{IntersectionPoints{vertices, normals, istart, iend,
                        nearPoint, farPoint, found, points}};

        IntersectionPoints{vertices, normals, iend, size, nearPoint, farPoint, found, points}();

        thread1.join();
        thread2.join();
        thread3.join();
    }

    auto time2 = std::chrono::steady_clock::now();
    LOG(INFO) << "intersect took: "
              << std::chrono::duration_cast<std::chrono::milliseconds>(time2 - time1).count();

    return found;
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

    if (buf.find("facet") != std::string::npos || buf.find("normal") != std::string::npos)
        return true;
    else
        return false;
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
