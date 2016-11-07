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
#include "Scene.h"
#include "Stl.h"
#include "StlModel.h"

namespace Geometry {

GeometryContainer::GeometryContainer(Model::Scene* scene)
    : m_scene{scene}
    , m_extent{}
{
    CHECK(m_scene) << "GeometryContainer::ctor: Model::Scene nullptr!";

    for (auto& modelObject : m_scene->modelObjectList())
        this->add(modelObject.get());

    m_scene->modelObjectAdded.connect<GeometryContainer, &GeometryContainer::add>(this);
}

GeometryContainer::~GeometryContainer()
{
    m_scene->modelObjectAdded.disconnect<GeometryContainer, &GeometryContainer::add>(this);

    for (auto& geometryObject : m_geometryObjectList)
        geometryObject->extentChanged.disconnect<GeometryContainer, &GeometryContainer::updateExtent>(this);
}

void GeometryContainer::add(Model::ModelObject* modelObject)
{
    if (!modelObject)
        return;

    auto geometryObjectPtr = this->makeGeometryObject(modelObject);
    if (!geometryObjectPtr)
        return;

    GeometryObject* geometryObject = geometryObjectPtr.get();
    m_geometryObjectList.push_back(std::move(geometryObjectPtr));

    geometryObject->extentChanged.connect<GeometryContainer, &GeometryContainer::updateExtent>(this);
    geometryObjectAdded.emit_signal(geometryObject); // emit signal

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

std::unique_ptr<GeometryObject> GeometryContainer::makeGeometryObject(Model::ModelObject* modelObject)
{
    std::unique_ptr<GeometryObject> geometryObjectPtr{};
    switch (modelObject->type()) {
    case Model::ModelObject::Type::Box:
        geometryObjectPtr = std::make_unique<Box>(*this, dynamic_cast<Model::BoxModel*>(modelObject));
        break;
    case Model::ModelObject::Type::Stl:
        geometryObjectPtr = std::make_unique<Stl>(*this, dynamic_cast<Model::StlModel*>(modelObject));
        break;
    }
    return geometryObjectPtr;
}

} // namespace Geometry
