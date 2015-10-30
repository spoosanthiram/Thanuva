/**
* Author: Saravanan Poosanthiram
* $LastChangedBy: ps $
* $LastChangedDate: 2015-03-31 17:41:12 -0400 (Tue, 31 Mar 2015) $
*/

#include "GfxStl.h"

#include <chrono>
#include <fstream>
#include <thread>

#include "glog/logging.h"

#include "Stl.h"

namespace GfxModel {

const std::size_t k2MillionTriangles = 2000000 * 9;
const std::size_t k4Point5MillionTriangles = 4500000 * 9;

GfxStl::GfxStl(const GfxProject& gfxProject, Model::Stl* stl)
    : GraphicsObject(gfxProject, stl)
{
    this->initialize();

    stl->geometryChanged.connect<GfxStl, &GfxStl::initialize>(this);
}

struct IntersectionPoints {
    IntersectionPoints(const std::vector<float>& vertices, const std::vector<float>& normals,
                       std::size_t istart, std::size_t iend,
                       const Core::Vector3d& nearPoint, const Core::Vector3d& farPoint,
                       volatile bool& found, std::vector<Core::Vector3d>* points)
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
        Core::Vector3d n, a, b, c, p;
        Core::Vector3d l = m_farPoint - m_nearPoint;

        for (size_t i = m_istart; i < m_iend; i += 9) {
            if (m_found && !m_points)
                break;

            a.assign(&m_vertices[i]);
            n.assign(&m_normals[i]);

            if (!this->intersect(a, n, m_nearPoint, l, p))
                continue;

            b.assign(&m_vertices[i + 3]);
            c.assign(&m_vertices[i + 6]);

            // is the point p inside the triangle?
            if (n.dot((b - a).cross(p - a)) >= 0.0 && n.dot((c - b).cross(p - b)) >= 0.0 && n.dot((a - c).cross(p - c)) >= 0.0) {
                m_found = true;
                if (m_points)
                    m_points->push_back(p);
                else
                    break;
            }
        }
    }

    bool intersect(const Core::Vector3d& a, const Core::Vector3d& n, const Core::Vector3d& nearPoint, const Core::Vector3d& l, Core::Vector3d& p)
    {
        double nDotL = n.dot(l);
        if (psa::iszero(nDotL)) // ray is parallel to plane (triangle) either starts outside or inside
            return false;

        double alpha = n.dot(a - nearPoint) / nDotL;
        if (alpha < 0.0 || alpha > 1.0) // plane is beyond the ray we consider
            return false;

        p = nearPoint + alpha * l; // // p intersect the plane (triangle)

        return true;
    }

    const std::vector<float>& m_vertices;
    const std::vector<float>& m_normals;
    std::size_t m_istart;
    std::size_t m_iend;
    const Core::Vector3d& m_nearPoint;
    const Core::Vector3d& m_farPoint;
    volatile bool& m_found;
    std::vector<Core::Vector3d>* m_points;
};

bool GfxStl::intersect(const Core::Vector3d& nearPoint, const Core::Vector3d& farPoint, std::vector<Core::Vector3d>* points)
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
        std::thread thread1{IntersectionPoints{vertices, normals, 0, size1, nearPoint, farPoint, found, points}};
        IntersectionPoints{vertices, normals, size1, size, nearPoint, farPoint, found, points}();
        thread1.join();
    }
    else {
        std::size_t sizeInc = size / 4;

        std::size_t istart = 0;
        std::size_t iend = sizeInc;
        std::thread thread1{IntersectionPoints{vertices, normals, istart, iend, nearPoint, farPoint, found, points}};

        istart = iend;
        iend = istart + sizeInc;
        std::thread thread2{IntersectionPoints{vertices, normals, istart, iend, nearPoint, farPoint, found, points}};

        istart = iend;
        iend = istart + sizeInc;
        std::thread thread3{IntersectionPoints{vertices, normals, istart, iend, nearPoint, farPoint, found, points}};

        IntersectionPoints{vertices, normals, iend, size, nearPoint, farPoint, found, points}();

        thread1.join();
        thread2.join();
        thread3.join();
    }

    auto time2 = std::chrono::steady_clock::now();
    LOG(INFO) << "intersect took: " << std::chrono::duration_cast<std::chrono::milliseconds>(time2 - time1).count();

    return found;
}

void GfxStl::initialize()
{
    this->clear();

    auto stl = dynamic_cast<Model::Stl*>(this->geometry());
    std::ifstream dataStream{stl->filePath(), std::ios::binary};

    auto dataStreamRdBuf = std::make_unique<char[]>(kDataStreamReadBufferSize);

    dataStream.rdbuf()->pubsetbuf(dataStreamRdBuf.get(), kDataStreamReadBufferSize);

    this->setExtent(Extent{
        std::numeric_limits<double>::infinity(),
        -std::numeric_limits<double>::infinity(),
        std::numeric_limits<double>::infinity(),
        -std::numeric_limits<double>::infinity(),
        std::numeric_limits<double>::infinity(),
        -std::numeric_limits<double>::infinity()
    });

    auto buf = std::make_unique<char[]>(kNFacetChunk * kFacetSize);

    // read STL header
    dataStream.read(buf.get(), kBinaryHeaderLength);

    // read number of facets
    dataStream.read(buf.get(), sizeof(int));
    int nFacets = *reinterpret_cast<int*>(buf.get());

    this->reserve(nFacets * GraphicsObject::kVerticesPerTriangle * GraphicsObject::kValuesPerVertex,
        nFacets * GraphicsObject::kVerticesPerTriangle * GraphicsObject::kValuesPerVertex, 0);

    int readSize = kNFacetChunk;

    for (int i = 0; i < nFacets; i += readSize) {
        readSize = ((nFacets - i) > kNFacetChunk) ? kNFacetChunk : (nFacets - i);

        dataStream.read(buf.get(), readSize * kFacetSize);

        char* p = buf.get();
        char* q = p + (readSize * kFacetSize);
        while (p < q) {
            this->insertNormal(reinterpret_cast<float*>(p));
            this->insertNormal(reinterpret_cast<float*>(p));
            this->insertNormal(reinterpret_cast<float*>(p));
            p += 3 * sizeof(float);

            this->insertVertex(reinterpret_cast<float*>(p));
            p += 3 * sizeof(float);
            this->insertVertex(reinterpret_cast<float*>(p));
            p += 3 * sizeof(float);
            this->insertVertex(reinterpret_cast<float*>(p));
            p += 3 * sizeof(float);

            p += sizeof(uint16_t);
        }
    }

    this->initializeBoundingBox();

    // emit signals
    graphicsObjectChanged();
    extentChanged();
}

} // namespace GfxModel
