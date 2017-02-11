/*
 * Geometry: Geometry objects for Thanuva
 *
 * Copyright 2016, Saravanan Poosanthiram
 * All rights reserved.
 */

#include "GeometryObject.h"

#include <chrono>

#include <glog/logging.h>

#include "ModelObject.h"

namespace {

const std::size_t k2MillionTriangles = 2000000 * 9;
const std::size_t k4Point5MillionTriangles = 4500000 * 9;

/**
* Checks for: does the line (np, l) intersects the plane (a, n).
* The line characterized by two point np and fp with l = fp - np.
* The plane of infinite dimension with 'a' that lies on the plane and 'n' normal to the plane.
* @return true if intersecting point is found. 'p' will be filled.
*         false if no intersecting point is found.
*/
bool intersectPlane(const Core::Point3d& a, const Core::Vector3d& n, const Core::Point3d& np,
                    const Core::Vector3d& l, Core::Point3d& p)
{
    double nDotL = n.dot(l);
    if (psa::iszero(nDotL)) // ray is parallel to plane (triangle) either starts outside or inside
        return false;

    double alpha = n.dot(a - np) / nDotL;
    if (alpha < 0.0 || alpha > 1.0) // plane is beyond the ray we consider
        return false;

    p = np + alpha * l; // p intersect the plane (triangle)

    return true;
}

} // anonymous namespace

namespace Geometry {

GeometryObject::GeometryObject(const GeometryContainer& geometryContainer,
                               Model::ModelObject* modelObject)
    : m_geometryContainer{geometryContainer}
    , m_modelObject{modelObject}
{
    CHECK(modelObject) << "GeometryObject::ctor: Model::ModelObject nullptr!";
}

void GeometryObject::setExtent(const Extent& extent, Core::EmitSignal emitSignal)
{
    if (extent == m_extent)
        return;

    m_extent = extent;
    if (Core::EmitSignal::Emit == emitSignal)
        extentChanged.emit_signal(); // emit signal
}

struct TriangleIntersection {
    TriangleIntersection(const GeometryObject& geometryObject, std::size_t startIndex, std::size_t endIndex,
                         const Core::Point3d& nearPoint, const Core::Point3d& farPoint,
                         std::vector<Core::Point3d>* points)
        : m_geometryObject{geometryObject}
        , m_startIndex{startIndex}
        , m_endIndex{endIndex}
        , m_nearPoint{nearPoint}
        , m_farPoint{farPoint}
        , m_points{points}
    {}

    bool operator()() {
        const std::vector<float>& vertices = m_geometryObject.vertices();
        const std::vector<float>& normals = m_geometryObject.normals();
        const std::vector<int>& indices = m_geometryObject.indices();

        bool found = false;

        Core::Vector3d n;
        Core::Point3d a, b, c, p;
        Core::Vector3d l = m_farPoint - m_nearPoint;

        for (size_t i = m_startIndex; i < m_endIndex; i += 3) {
            a.assign(&vertices[indices[i] * 3]);
            n.assign(&normals[indices[i] * 3]);

            if (!intersectPlane(a, n, m_nearPoint, l, p))
                continue;

            b.assign(&vertices[indices[i + 1] * 3]);
            c.assign(&vertices[indices[i + 2] * 3]);

            // is the point p inside the triangle?
            if (n.dot((b - a).cross(p - a)) >= 0.0 &&
                n.dot((c - b).cross(p - b)) >= 0.0 &&
                n.dot((a - c).cross(p - c)) >= 0.0)
            {
                found = true;
                if (m_points)
                    m_points->push_back(p);
                else
                    break;
            }
        }

        return found;
    }

    const GeometryObject& m_geometryObject;
    std::size_t m_startIndex;
    std::size_t m_endIndex;
    const Core::Point3d& m_nearPoint;
    const Core::Point3d& m_farPoint;
    std::vector<Core::Point3d>* m_points;
};

bool GeometryObject::intersect(const Core::Point3d& nearPoint, const Core::Point3d& farPoint,
                               std::vector<Core::Point3d>* points) const
{
    if (this->vertices().size() == 0 ||
        this->vertices().size() != this->normals().size() ||
        this->indices().size() == 0)
    {
        return false;
    }

    bool found = false;
    auto time1 = std::chrono::steady_clock::now();

    std::size_t size = this->vertices().size();
    if (size <= k2MillionTriangles)
        found = TriangleIntersection{*this, 0, size, nearPoint, farPoint, points}();
    //else if (size <= k4Point5MillionTriangles) {
    //    std::size_t size1 = size / 2;
    //    std::thread thread1{IntersectionPoints{vertices, normals, 0, size1,
    //        nearPoint, farPoint, found, points}};
    //    IntersectionPoints{vertices, normals, size1, size, nearPoint, farPoint, found, points}();
    //    thread1.join();
    //}
    //else {
    //    std::size_t sizeInc = size / 4;

    //    std::size_t istart = 0;
    //    std::size_t iend = sizeInc;
    //    std::thread thread1{IntersectionPoints{vertices, normals, istart, iend,
    //        nearPoint, farPoint, found, points}};

    //    istart = iend;
    //    iend = istart + sizeInc;
    //    std::thread thread2{IntersectionPoints{vertices, normals, istart, iend,
    //        nearPoint, farPoint, found, points}};

    //    istart = iend;
    //    iend = istart + sizeInc;
    //    std::thread thread3{IntersectionPoints{vertices, normals, istart, iend,
    //        nearPoint, farPoint, found, points}};

    //    IntersectionPoints{vertices, normals, iend, size, nearPoint, farPoint, found, points}();

    //    thread1.join();
    //    thread2.join();
    //    thread3.join();
    //}

    auto time2 = std::chrono::steady_clock::now();
    LOG(INFO) << "intersect took: "
        << std::chrono::duration_cast<std::chrono::milliseconds>(time2 - time1).count();

    return found;
}

bool GeometryObject::intersectBoundingBox(const Core::Point3d& nearPoint, const Core::Point3d& farPoint) const
{
    const unsigned int kNumQuad = 6;

    Core::Vector3d n;
    Core::Point3d a, b, c, d, p;
    Core::Vector3d l = farPoint - nearPoint;

    for (unsigned int i = 0, index; i < kNumQuad; ++i) {
        index = i * 4;

        a.assign(&m_boundingBoxVertices[m_boundingBoxIndices[index]]);
        n.assign(&m_boundingBoxNormals[i * 3]);

        if (!intersectPlane(a, n, nearPoint, l, p))
            continue;

        b.assign(&m_boundingBoxVertices[m_boundingBoxIndices[index + 1]]);
        c.assign(&m_boundingBoxVertices[m_boundingBoxIndices[index + 2]]);
        d.assign(&m_boundingBoxVertices[m_boundingBoxIndices[index + 3]]);

        // is the point p inside the quad?
        if (n.dot((b - a).cross(p - a)) >= 0.0 && n.dot((c - b).cross(p - b)) >= 0.0 &&
            n.dot((d - c).cross(p - c)) >= 0.0 && n.dot((a - d).cross(p - d)) >= 0)
        {
            return true;
        }
    }

    return false;
}

void GeometryObject::initializeBoundingBox()
{
    if (m_extent.isAnyInfinite())
        return;

    Core::Point3d a{m_extent.xMin(), m_extent.yMin(), m_extent.zMin()};
    Core::Point3d b{m_extent.xMax(), m_extent.yMin(), m_extent.zMin()};
    Core::Point3d c{m_extent.xMax(), m_extent.yMax(), m_extent.zMin()};
    Core::Point3d d{m_extent.xMin(), m_extent.yMax(), m_extent.zMin()};
    Core::Point3d e{m_extent.xMin(), m_extent.yMin(), m_extent.zMax()};
    Core::Point3d f{m_extent.xMax(), m_extent.yMin(), m_extent.zMax()};
    Core::Point3d g{m_extent.xMax(), m_extent.yMax(), m_extent.zMax()};
    Core::Point3d h{m_extent.xMin(), m_extent.yMax(), m_extent.zMax()};

    this->insertBoundingBoxVertex(0, a);
    this->insertBoundingBoxVertex(3, b);
    this->insertBoundingBoxVertex(6, c);
    this->insertBoundingBoxVertex(9, d);
    this->insertBoundingBoxVertex(12, e);
    this->insertBoundingBoxVertex(15, f);
    this->insertBoundingBoxVertex(18, g);
    this->insertBoundingBoxVertex(21, h);

    this->insertBoundingBoxQuadIndices(0, 3, 6, 18, 21); //X Max plane (b, c, g, f)
    this->insertBoundingBoxNormal(0, this->computeNormal(b, c, g));

    this->insertBoundingBoxQuadIndices(4, 9, 0, 12, 21); // X Min plane (d, a, e, h)
    this->insertBoundingBoxNormal(3, this->computeNormal(d, a, e));

    this->insertBoundingBoxQuadIndices(8, 6, 9, 21, 18); // Y Max plane (c, d, h, g)
    this->insertBoundingBoxNormal(6, this->computeNormal(c, d, h));

    this->insertBoundingBoxQuadIndices(12, 0, 3, 15, 12); // Y Min plane (a, b, f, e)
    this->insertBoundingBoxNormal(9, this->computeNormal(a, b, f));

    this->insertBoundingBoxQuadIndices(16, 12, 15, 18, 21); // Z Max plane (e, f, g, h)
    this->insertBoundingBoxNormal(12, this->computeNormal(e, f, g));

    this->insertBoundingBoxQuadIndices(20, 9, 6, 3, 0); // Z Min plane (d, c, b, a)
    this->insertBoundingBoxNormal(15, this->computeNormal(d, c, b));
}

} // namespace Geometry
