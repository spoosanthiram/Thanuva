/**
* Author: Saravanan Poosanthiram
* $LastChangedBy: ps $
* $LastChangedDate: 2015-03-29 02:53:27 -0400 (Sun, 29 Mar 2015) $
*/

#include "GraphicsObject.h"

#include "ModelObject.h"

#include "glog/logging.h"

namespace GfxModel {

GraphicsObject::GraphicsObject(const GfxProject& gfxProject, Model::ModelObject* modelObject)
    : m_gfxProject{gfxProject}
    , m_modelObject{modelObject}
    , m_vertices{}
    , m_normals{}
    , m_indices{}
    , m_extent{}
    , m_boundingBoxVertices{}
    , m_boundingBoxNormals{}
{
    CHECK(modelObject) << "GraphicsObject::ctor: Model::ModelObject nullptr!";
}

void GraphicsObject::setExtent(const Extent& extent, Core::EmitSignal emitSignal)
{
    if (extent == m_extent)
        return;

    m_extent = extent;
    if (Core::EmitSignal::Emit == emitSignal)
        extentChanged.emit_signal(); // emit signal
}

bool GraphicsObject::intersectBoundingBox(const Core::Vector3d& nearPoint, const Core::Vector3d& farPoint)
{
    const unsigned int kNumQuad = 6;

    Core::Vector3d a, b, c, d, n, p;
    Core::Vector3d l = farPoint - nearPoint;

    for (unsigned int i = 0, index; i < kNumQuad; ++i) {
        index = i * 4;

        a.assign(&m_boundingBoxVertices[m_boundingBoxIndices[index]]);
        n.assign(&m_boundingBoxNormals[i * 3]);

        if (!this->intersect(a, n, nearPoint, l, p))
            continue;
            
        b.assign(&m_boundingBoxVertices[m_boundingBoxIndices[index + 1]]);
        c.assign(&m_boundingBoxVertices[m_boundingBoxIndices[index + 2]]);
        d.assign(&m_boundingBoxVertices[m_boundingBoxIndices[index + 3]]);

        // is the point p inside the quad?
        if (n.dot((b - a).cross(p - a)) >= 0.0 && n.dot((c - b).cross(p - b)) >= 0.0
                && n.dot((d - c).cross(p - c)) >= 0.0 && n.dot((a - d).cross(p - d)) >= 0)
            return true;
    }

    return false;
}

void GraphicsObject::insertQuad(const Core::Vector3d& a, const Core::Vector3d& b,
                                const Core::Vector3d& c, const Core::Vector3d& d)
{
    int index = static_cast<int>(m_vertices.size() / kValuesPerVertex);

    this->insertVertex(a);
    this->insertVertex(b);
    this->insertVertex(c);
    this->insertVertex(d);

    Core::Vector3d n = this->computeNormal(a, b, c);

    this->insertNormal(n);
    this->insertNormal(n);
    this->insertNormal(n);
    this->insertNormal(n);

    // a, b, c triangle
    m_indices.push_back(index);
    m_indices.push_back(index + 1);
    m_indices.push_back(index + 2);

    // a, c, d triangle
    m_indices.push_back(index);
    m_indices.push_back(index + 2);
    m_indices.push_back(index + 3);
}

void GraphicsObject::initializeBoundingBox()
{
    if (m_extent.isAnyInfinite())
        return;

    Core::Vector3d a{m_extent.xMin(), m_extent.yMin(), m_extent.zMin()};
    Core::Vector3d b{m_extent.xMax(), m_extent.yMin(), m_extent.zMin()};
    Core::Vector3d c{m_extent.xMax(), m_extent.yMax(), m_extent.zMin()};
    Core::Vector3d d{m_extent.xMin(), m_extent.yMax(), m_extent.zMin()};
    Core::Vector3d e{m_extent.xMin(), m_extent.yMin(), m_extent.zMax()};
    Core::Vector3d f{m_extent.xMax(), m_extent.yMin(), m_extent.zMax()};
    Core::Vector3d g{m_extent.xMax(), m_extent.yMax(), m_extent.zMax()};
    Core::Vector3d h{m_extent.xMin(), m_extent.yMax(), m_extent.zMax()};

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

} // namespace GfxModel
