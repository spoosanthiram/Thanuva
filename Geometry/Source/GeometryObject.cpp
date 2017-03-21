/*
 * Geometry: Geometry objects for Thanuva
 *
 * Copyright 2016, Saravanan Poosanthiram
 * All rights reserved.
 */

#include "GeometryObject.h"

#include <chrono>
#include <thread>

#include <glog/logging.h>

#include "ModelObject.h"

namespace {

const std::size_t kNoThreadSize = 1000000 * 3;
const std::size_t kTwoThreadSize = 3000000 * 3;

} // anonymous namespace

namespace Geometry {

GeometryObject::GeometryObject(const GeometryContainer* geometryContainer,
                               Model::ModelObject* modelObject)
    : m_geometryContainer{geometryContainer}
    , m_modelObject{modelObject}
{
    CHECK(modelObject) << "GeometryObject::ctor: Model::ModelObject nullptr!";

    this->updateTransformMatrix();

    m_modelObject->transformChanged.connect<GeometryObject, &GeometryObject::updateTransformMatrix>(this);
}

bool GeometryObject::setTransformMatrix(const Core::Matrix4x4& transformMatrix,
                                        Core::EmitSignal emitSignal)
{
    if (m_transformMatrix == transformMatrix)
        return false;

    m_transformMatrix = transformMatrix;
    if (Core::EmitSignal::Emit == emitSignal)
        transformMatrixChanged.emit_signal(); // emit signal
    return true;
}

void GeometryObject::setExtent(const Extent& extent, Core::EmitSignal emitSignal)
{
    if (m_extent == extent)
        return;

    m_extent = extent;
    if (Core::EmitSignal::Emit == emitSignal)
        extentChanged.emit_signal(); // emit signal
}

bool GeometryObject::intersect(const Core::Point3d& nearPoint, const Core::Point3d& farPoint)
{
    if (this->vertices().size() == 0 || this->indices().size() == 0)
        return false;

    m_probePoints.clear(); // Remove old probe points

    auto t1 = std::chrono::steady_clock::now();

    std::size_t size = this->indices().size();
    if (size <= kNoThreadSize)
        this->intersectInternal(nearPoint, farPoint, 0, size);
    else {
        int numThreads = (size > kTwoThreadSize) ? 4 : 2;
        std::size_t incrementSize = (((size / 3) / numThreads) + 1) * 3;

        std::vector<std::size_t> intervals;
        for (int i = 0; i < numThreads; ++i)
            intervals.push_back(i * incrementSize);
        intervals.push_back(size);

        std::vector<std::thread> threads;
        for (int i = 0; i < numThreads; ++i)
            threads.emplace_back(&GeometryObject::intersectInternal, this,
                                 std::cref(nearPoint), std::cref(farPoint), intervals[i], intervals[i + 1]);
        for (auto& t : threads)
            t.join();
    }

    auto t2 = std::chrono::steady_clock::now();
    LOG(INFO) << "intersect took: " << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();

    return m_probePoints.size() > 0;
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

        if (!this->intersectPlane(a, n, nearPoint, l, p))
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

void GeometryObject::updateExtent()
{
    Extent extent = m_boundingBox;
    extent.transform(m_transformMatrix);
    this->setExtent(extent);
}

void GeometryObject::initializeBoundingBox()
{
    if (m_boundingBox.isAnyInfinite())
        return;

    Core::Point3d a{m_boundingBox.xMin(), m_boundingBox.yMin(), m_boundingBox.zMin()};
    Core::Point3d b{m_boundingBox.xMax(), m_boundingBox.yMin(), m_boundingBox.zMin()};
    Core::Point3d c{m_boundingBox.xMax(), m_boundingBox.yMax(), m_boundingBox.zMin()};
    Core::Point3d d{m_boundingBox.xMin(), m_boundingBox.yMax(), m_boundingBox.zMin()};
    Core::Point3d e{m_boundingBox.xMin(), m_boundingBox.yMin(), m_boundingBox.zMax()};
    Core::Point3d f{m_boundingBox.xMax(), m_boundingBox.yMin(), m_boundingBox.zMax()};
    Core::Point3d g{m_boundingBox.xMax(), m_boundingBox.yMax(), m_boundingBox.zMax()};
    Core::Point3d h{m_boundingBox.xMin(), m_boundingBox.yMax(), m_boundingBox.zMax()};

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

void GeometryObject::updateTransformMatrix()
{
    auto modelXfrom = m_modelObject->transform();

    Core::Matrix4x4 xformMatrix = Core::Matrix4x4::identity();
    xformMatrix.translate(modelXfrom.translateX, modelXfrom.translateY, modelXfrom.translateZ);
    if (this->setTransformMatrix(xformMatrix))
        this->updateExtent();
}

void GeometryObject::intersectInternal(const Core::Point3d& nearPoint, const Core::Point3d& farPoint,
                                       std::size_t startIndex, std::size_t endIndex)
{
    bool threaded = m_indices.size() != (endIndex - startIndex);

    Core::Vector3d n;
    Core::Point3d a, b, c, p;

    Core::Vector3d l = farPoint - nearPoint;

    for (size_t i = startIndex; i < endIndex; i += 3) {
        a.assign(&m_vertices[m_indices[i] * 3]);
        n.assign(&m_normals[m_indices[i] * 3]);

        if (!intersectPlane(a, n, nearPoint, l, p))
            continue;

        b.assign(&m_vertices[m_indices[i + 1] * 3]);
        c.assign(&m_vertices[m_indices[i + 2] * 3]);

        // is the point p inside the triangle?
        if (n.dot((b - a).cross(p - a)) >= 0.0 &&
            n.dot((c - b).cross(p - b)) >= 0.0 &&
            n.dot((a - c).cross(p - c)) >= 0.0)
        {
            if (threaded) {
                std::lock_guard<std::mutex> lg{m_probePointsMutex};
                m_probePoints.push_back(p);
            }
            else
                m_probePoints.push_back(p);
        }
    }
}

} // namespace Geometry
