/*
 * Geometry: Geometry objects for Thanuva
 *
 * Copyright 2016, Saravanan Poosanthiram
 * All rights reserved.
 */

#ifndef GEOMETRY_GEOMETRYOBJECT_H
#define GEOMETRY_GEOMETRYOBJECT_H

#include <array>
#include <memory>
#include <mutex>
#include <vector>

#include <nano_signal_slot.hpp>

#include "CoreDef.h"
#include "Extent.h"
#include "Point3d.h"
#include "Vector3d.h"

namespace Model { class ModelObject; }

namespace Geometry {

class GeometryContainer;

class GeometryObject
{
public:
    static const int kValuesPerVertex = 3;
    static const int kVerticesPerTriangle = 3;

public:
    GeometryObject(const GeometryContainer* geometryContainer, Model::ModelObject* modelObject);
    ~GeometryObject() {}

    Model::ModelObject* modelObject() const { return m_modelObject; }
    const std::vector<float>& vertices() const { return m_vertices; }
    const std::vector<float>& normals() const { return m_normals; }
    const std::vector<int>& indices() const { return m_indices; }
    const Extent& extent() const { return m_extent; }
    const std::vector<Core::Point3d>& probePoints() const { return m_probePoints; }

    void setExtent(const Extent& extent, Core::EmitSignal emitSignal = Core::EmitSignal::Emit);

    bool intersect(const Core::Point3d& nearPoint, const Core::Point3d& farPoint);
    bool intersectBoundingBox(const Core::Point3d& nearPoint, const Core::Point3d& farPoint) const;

public: // signals
    Nano::Signal<void()> geometryObjectChanged{};
    Nano::Signal<void()> extentChanged{};

protected:
    void insertVertex(const float* vertex)
    {
        m_vertices.push_back(vertex[0]);
        m_vertices.push_back(vertex[1]);
        m_vertices.push_back(vertex[2]);

        m_extent.update(vertex);
    }
    void insertVertex(const Core::Point3d& vertex)
    {
        m_vertices.push_back(static_cast<float>(vertex.x()));
        m_vertices.push_back(static_cast<float>(vertex.y()));
        m_vertices.push_back(static_cast<float>(vertex.z()));

        m_extent.update(vertex);
    }
    void duplicateVertices(size_t startIndex, size_t len)
    {
        //auto first = m_vertices.begin() + startIndex;
        //auto last = first + len;
        //std::copy(first, last, std::back_inserter(m_vertices));
        for (size_t i = 0; i < len; ++i)
            m_vertices.push_back(m_vertices[startIndex + i]);
    }

    void insertNormal(const float* normal)
    {
        m_normals.push_back(normal[0]);
        m_normals.push_back(normal[1]);
        m_normals.push_back(normal[2]);
    }
    void insertNormal(const Core::Vector3d& normal)
    {
        m_normals.push_back(static_cast<float>(normal.x()));
        m_normals.push_back(static_cast<float>(normal.y()));
        m_normals.push_back(static_cast<float>(normal.z()));
    }

    void insertIndices(int ia, int ib, int ic)
    {
        m_indices.push_back(ia);
        m_indices.push_back(ib);
        m_indices.push_back(ic);
    }

    Core::Vector3d computeNormal(const Core::Point3d& a, const Core::Point3d& b, const Core::Point3d& c)
    {
        Core::Vector3d n = (b - a).cross(c - a);
        n.normalize();
        return n;
    }

    void reserve(std::size_t nvertices, std::size_t nnormals, std::size_t nindices)
    {
        m_vertices.reserve(nvertices);
        m_normals.reserve(nnormals);
        m_indices.reserve(nindices);
    }
    void clear()
    {
        m_vertices.clear();
        m_normals.clear();
        m_indices.clear();
    }

    void insertBoundingBoxVertex(std::size_t index, const Core::Point3d& v)
    {
        m_boundingBoxVertices[index] = static_cast<float>(v.x());
        m_boundingBoxVertices[index + 1] = static_cast<float>(v.y());
        m_boundingBoxVertices[index + 2] = static_cast<float>(v.z());
    }
    void insertBoundingBoxNormal(std::size_t index, const Core::Vector3d& n)
    {
        m_boundingBoxNormals[index] = static_cast<float>(n.x());
        m_boundingBoxNormals[index + 1] = static_cast<float>(n.y());
        m_boundingBoxNormals[index + 2] = static_cast<float>(n.z());
    }
    void insertBoundingBoxQuadIndices(unsigned int index,
                                      unsigned short aIndex,
                                      unsigned short bIndex,
                                      unsigned short cIndex,
                                      unsigned short dIndex)
    {
        m_boundingBoxIndices[index] = aIndex;
        m_boundingBoxIndices[index + 1] = bIndex;
        m_boundingBoxIndices[index + 2] = cIndex;
        m_boundingBoxIndices[index + 3] = dIndex;
    }

    void initializeBoundingBox();

private:
    void intersectInternal(const Core::Point3d& nearPoint, const Core::Point3d& farPoint,
                           std::size_t startIndex, std::size_t endIndex);
    /**
    * Checks for: does the line (np, l) intersects the plane (a, n).
    * The line characterized by two point np and fp with l = fp - np.
    * The plane of infinite dimension with 'a' that lies on the plane and 'n' normal to the plane.
    * @return true if intersecting point is found. 'p' will be filled.
    *         false if no intersecting point is found.
    */
    bool intersectPlane(const Core::Point3d& a, const Core::Vector3d& n, const Core::Point3d& np,
                        const Core::Vector3d& l, Core::Point3d& p) const
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

private:
    const GeometryContainer* m_geometryContainer;

    Model::ModelObject* m_modelObject;

    std::vector<float> m_vertices{};
    std::vector<float> m_normals{};
    std::vector<int> m_indices{};
    Extent m_extent{};

    std::array<float, 24> m_boundingBoxVertices{};
    std::array<float, 18> m_boundingBoxNormals{};
    std::array<unsigned short, 24> m_boundingBoxIndices{};

    std::vector<Core::Point3d> m_probePoints{};
    std::mutex m_probePointsMutex{};
};

} // namespace Geometry

#endif // GEOMETRY_GEOMETRYOBJECT_H
