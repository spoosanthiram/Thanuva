/*
 * Geometry: Geometry objects for Thanuva
 *
 * Copyright 2016, Saravanan Poosanthiram
 * All rights reserved.
 */

#include "GeometryContainer.h"

#include <glog/logging.h>

#include "Box.h"
#include "BoxModel.h"
#include "Project.h"
#include "Stl.h"
#include "StlModel.h"

namespace Geometry {

const double GeometryContainer::kDefaultWindowAspect = 1.6;

GeometryContainer::GeometryContainer(Model::Project* project)
    : m_project{project}
    , m_viewpointCamera{*this, project->viewpointCameraModel()}
    , m_extent{}
    , m_windowAspect{kDefaultWindowAspect}
    , m_projectionMatrix{Core::Matrix4x4::identity()}
{
    CHECK(m_project) << "GeometryContainer::ctor: Model::Project nullptr!";

    for (auto& modelObject : m_project->modelObjectList())
        this->add(modelObject.get());

    m_project->modelObjectAdded.connect<GeometryContainer, &GeometryContainer::add>(this);
}

GeometryContainer::~GeometryContainer()
{
    m_project->modelObjectAdded.disconnect<GeometryContainer, &GeometryContainer::add>(this);

    for (auto graphicsObject : m_graphicsObjectList) {
        graphicsObject->extentChanged.disconnect<GeometryContainer, &GeometryContainer::updateExtent>(this);
        delete graphicsObject;
    }
}

void GeometryContainer::adjustProjection(int width, int height)
{
    m_windowAspect = static_cast<double>(width) / static_cast<double>(height);
    this->updateProjectionMatrix();
}

void GeometryContainer::add(Model::Geometry* modelObject)
{
    if (!modelObject)
        return;

    GeometryObject* graphicsObject = nullptr;

    switch (modelObject->type()) {
    case Model::Geometry::Type::Box:
        graphicsObject = new GfxBox{*this, dynamic_cast<Model::Box*>(modelObject)};
        break;
    case Model::Geometry::Type::Stl:
        graphicsObject = new GfxStl{*this, dynamic_cast<Model::Stl*>(modelObject)};
        break;
    }

    if (!graphicsObject)
        return;

    m_graphicsObjectList.push_back(graphicsObject);
    graphicsObject->extentChanged.connect<GeometryContainer, &GeometryContainer::updateExtent>(this);
    graphicsObjectAdded.emit_signal(*graphicsObject); // emit signal

    this->updateExtent();
}

void GeometryContainer::updateExtent()
{
    Extent extent;
    for (const auto& object : m_graphicsObjectList)
        extent.update(object->extent());

    if (m_extent != extent) {
        m_extent = extent;
        m_project->viewpointCameraModel().setViewportTranslation(m_extent.maxLength() * 2.0);
        this->updateProjectionMatrix();
        extentChanged.emit_signal(); // emit signal
    }
}

void GeometryContainer::updateProjectionMatrix()
{
    m_projectionMatrix = Core::Matrix4x4::perspective(50.0, m_windowAspect, 0.1, m_extent.maxLength() * 3.0);
}

} // namespace Geometry
