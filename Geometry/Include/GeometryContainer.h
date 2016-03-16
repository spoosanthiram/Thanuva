/*
 * Geometry: Geometry objects for Thanuva
 *
 * Copyright 2016, Saravanan Poosanthiram
 * All rights reserved.
 */

#ifndef GEOMETRY_GEOMETRYCONTAINER_H
#define GEOMETRY_GEOMETRYCONTAINER_H

#include <memory>
#include <vector>

#include <nano_signal_slot.hpp>

#include "GeometryObject.h"
#include "ViewpointCamera.h"

namespace Model {
    class ModelObject;
    class Project;
}

namespace Geometry {

class GeometryContainer {
public:
    static const double kDefaultWindowAspect;

public:
    GeometryContainer(Model::Project* project);
    GeometryContainer(const GeometryContainer& other) = delete;
    ~GeometryContainer();

    GeometryContainer& operator=(const GeometryContainer& other) = delete;

    const std::vector<GeometryObject*>& graphicsObjectList() const { return m_graphicsObjectList; }
    const ViewpointCamera& viewpointCamera() const { return m_viewpointCamera; }
    ViewpointCamera& viewpointCamera() { return m_viewpointCamera; }
    const Extent& extent() const { return m_extent; }
    const Core::Matrix4x4& projectionMatrix() const { return m_projectionMatrix; }

    void adjustProjection(int width, int height);
    void add(Model::ModelObject* modelObject);

public: // signals
    Nano::Signal<void(GeometryObject&)> geometryObjectAdded{};
    Nano::Signal<void()> extentChanged{};

protected: // slots
    void updateExtent();

private:
    void updateProjectionMatrix();

    Model::Project* m_project;

    std::vector<GeometryObject*> m_geometryObjectList{};
    ViewpointCamera m_viewpointCamera;
    Extent m_extent;
    double m_windowAspect;
    Core::Matrix4x4 m_projectionMatrix;
};

} // namespace Geometry

#endif // GEOMETRY_GEOMETRYCONTAINER_H
