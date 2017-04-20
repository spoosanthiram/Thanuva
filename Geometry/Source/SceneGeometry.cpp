/*
 * Geometry: Geometry objects for Thanuva
 *
 * Copyright 2016, Saravanan Poosanthiram
 * All rights reserved.
 */

#include "SceneGeometry.h"

#include <unordered_map>

#include <glog/logging.h>

#include "Box.h"
#include "BoxModel.h"
#include "Cone.h"
#include "ConeModel.h"
#include "Cylinder.h"
#include "CylinderModel.h"
#include "Mesh.h"
#include "MeshModel.h"
#include "ModelException.h"
#include "Scene.h"
#include "Sphere.h"
#include "SphereModel.h"

namespace Geometry {

SceneGeometry::SceneGeometry(Model::Scene* scene)
    : m_scene{scene}
    , m_extent{}
{
    CHECK(m_scene);

    for (auto& csysModel : m_scene->coordinateSystemModelList())
        this->add(csysModel.get());

    for (auto& modelObject : m_scene->geometryModelList())
        this->add(modelObject.get());

    m_scene->coordinateSystemModelAdded.connect<SceneGeometry, &SceneGeometry::add>(this);
    m_scene->geometryModelAdded.connect<SceneGeometry, &SceneGeometry::add>(this);
}

SceneGeometry::~SceneGeometry()
{
    //m_scene->coordinateSystemModelAdded.disconnect<SceneGeometry, &SceneGeometry::add>(this);
    //m_scene->geometryModelAdded.disconnect<SceneGeometry, &SceneGeometry::add>(this);

    //for (auto& geometryObject : m_geometryList)
    //    geometryObject->extentChanged.disconnect<SceneGeometry, &SceneGeometry::updateExtent>(this);
}

const CoordinateSystem* SceneGeometry::coordinateSystem(const Model::CoordinateSystemModel* csysModel) const
{
    auto it = std::find_if(m_coordinateSystemList.begin(), m_coordinateSystemList.end(),
            [=](const std::unique_ptr<CoordinateSystem>& csys) { return csys->thanuvaModel() == csysModel; });
    return (it != m_coordinateSystemList.end()) ? it->get() : nullptr;
}

void SceneGeometry::add(Model::CoordinateSystemModel* csysModel)
{
    auto coordinateSystem = std::make_unique<CoordinateSystem>(this, csysModel);
    m_coordinateSystemList.push_back(std::move(coordinateSystem));
}

void SceneGeometry::add(Model::GeometryModel* geometryModel)
{
    auto geometry = this->makeGeometry(geometryModel);
    Geometry* rawPtr = geometry.get();
    m_geometryList.push_back(std::move(geometry));

    rawPtr->extentChanged.connect<SceneGeometry, &SceneGeometry::updateExtent>(this);
    geometryAdded.emit_signal(rawPtr); // emit signal

    this->updateExtent();
}

void SceneGeometry::updateExtent()
{
    Extent extent;
    for (const auto& geometry : m_geometryList)
        extent.update(geometry->extent());

    if (m_extent != extent) {
        m_extent = extent;
        extentChanged.emit_signal(); // emit signal
    }
}

std::unique_ptr<Geometry> SceneGeometry::makeGeometry(Model::GeometryModel* geometryModel)
{
    std::unordered_map<std::string, std::function<std::unique_ptr<Geometry>(SceneGeometry*, Model::GeometryModel*)>> geometryMakerMap;
    geometryMakerMap[Model::BoxModel::kType] = [](SceneGeometry* sg, Model::GeometryModel* gm) -> std::unique_ptr<Geometry>
        { return std::make_unique<Box>(sg, dynamic_cast<Model::BoxModel*>(gm)); };
    geometryMakerMap[Model::CylinderModel::kType] = [](SceneGeometry* sg, Model::GeometryModel* gm) -> std::unique_ptr<Geometry>
        { return std::make_unique<Cylinder>(sg, dynamic_cast<Model::CylinderModel*>(gm)); };
    geometryMakerMap[Model::ConeModel::kType] = [](SceneGeometry* sg, Model::GeometryModel* gm) -> std::unique_ptr<Geometry>
        { return std::make_unique<Cone>(sg, dynamic_cast<Model::ConeModel*>(gm)); };
    geometryMakerMap[Model::SphereModel::kType] = [](SceneGeometry* sg, Model::GeometryModel* gm) -> std::unique_ptr<Geometry>
        { return std::make_unique<Sphere>(sg, dynamic_cast<Model::SphereModel*>(gm)); };
    geometryMakerMap[Model::MeshModel::kType] = [](SceneGeometry* sg, Model::GeometryModel* gm) -> std::unique_ptr<Geometry>
        { return std::make_unique<Mesh>(sg, dynamic_cast<Model::MeshModel*>(gm)); };

    auto it = geometryMakerMap.find(geometryModel->type());
    if (it == geometryMakerMap.end())
        throw Model::ModelException{Model::ModelException::kInvalidType};

    return it->second(this, geometryModel);
}

} // namespace Geometry
