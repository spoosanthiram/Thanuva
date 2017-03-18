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
#include "Cylinder.h"
#include "CylinderModel.h"
#include "Mesh.h"
#include "MeshModel.h"
#include "Scene.h"

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
    std::function<std::unique_ptr<GeometryObject>(GeometryContainer*, Model::ModelObject*)> geometryMaker[] =
    {
        [](GeometryContainer* gc, Model::ModelObject* mo) -> std::unique_ptr<GeometryObject>
            { return std::make_unique<Box>(gc, dynamic_cast<Model::BoxModel*>(mo)); },
        [](GeometryContainer* gc, Model::ModelObject* mo) -> std::unique_ptr<GeometryObject>
            { return std::make_unique<Mesh>(gc, dynamic_cast<Model::MeshModel*>(mo)); },
        [](GeometryContainer* gc, Model::ModelObject* mo) -> std::unique_ptr<GeometryObject>
            { return std::make_unique<Cylinder>(gc, dynamic_cast<Model::CylinderModel*>(mo)); }
    };

    return geometryMaker[static_cast<int>(modelObject->type())](this, modelObject);
}

} // namespace Geometry
