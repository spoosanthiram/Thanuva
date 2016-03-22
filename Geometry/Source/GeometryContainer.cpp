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

GeometryContainer::GeometryContainer(Model::Project* project)
    : m_project{project}
    , m_extent{}
{
    CHECK(m_project) << "GeometryContainer::ctor: Model::Project nullptr!";

    for (auto& modelObject : m_project->modelObjectList())
        this->add(modelObject.get());

    m_project->modelObjectAdded.connect<GeometryContainer, &GeometryContainer::add>(this);
}

GeometryContainer::~GeometryContainer()
{
    m_project->modelObjectAdded.disconnect<GeometryContainer, &GeometryContainer::add>(this);

    for (auto geometryObject : m_geometryObjectList) {
        geometryObject->extentChanged.
                disconnect<GeometryContainer, &GeometryContainer::updateExtent>(this);
        delete geometryObject;
    }
}

void GeometryContainer::add(Model::ModelObject* modelObject)
{
    if (!modelObject)
        return;

    GeometryObject* geometryObject = nullptr;

    switch (modelObject->type()) {
    case Model::ModelObject::Type::Box:
        geometryObject = new Box{*this, dynamic_cast<Model::BoxModel*>(modelObject)};
        break;
    case Model::ModelObject::Type::Stl:
        geometryObject = new Stl{*this, dynamic_cast<Model::StlModel*>(modelObject)};
        break;
    }

    if (!geometryObject)
        return;

    m_geometryObjectList.push_back(geometryObject);
    geometryObject->extentChanged.
            connect<GeometryContainer, &GeometryContainer::updateExtent>(this);
    geometryObjectAdded.emit_signal(*geometryObject); // emit signal

    this->updateExtent();
}

void GeometryContainer::updateExtent()
{
    Extent extent;
    for (const auto& object : m_geometryObjectList)
        extent.update(object->extent());

    if (m_extent != extent) {
        m_extent = extent;
        extentChanged.emit_signal(); // emit signal
    }
}

} // namespace Geometry
