/*
 * Model: Model objects for Thanuva
 *
 * Copyright 2016, Saravanan Poosanthiram
 * All rights reserved.
 */

#include "Scene.h"

#include <unordered_map>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <fmt/format.h>

#include "BoxModel.h"
#include "ConeModel.h"
#include "CylinderModel.h"
#include "MeshModel.h"
#include "ModelException.h"
#include "SphereModel.h"

namespace {

const char* kCoordinateSystemsTag = "coordinateSystems";
const char* kGeometryModelsTag = "geometryModels";
const char* kViewpointsTag = "viewpoints";
const char* kModelObjectTag = "modelobject";

} // anonymous

using boost::property_tree::ptree;

namespace Model {

const char* Scene::kDefaultName = "New Scene";

Scene::Scene(const ThanuvaApp& thanuvaApp)
    : m_thanuvaApp{thanuvaApp}
    , m_name{kDefaultName}
    , m_filePath{}
{
    m_csysModelList.push_back(std::make_unique<CoordinateSystemModel>(this, "world_csys"));
    m_viewpointList.push_back(std::make_unique<Viewpoint>(this));
}

Scene::Scene(const ThanuvaApp& thanuvaApp, const fs::path& filePath)
    : m_thanuvaApp{thanuvaApp}
    , m_name{/*m_filePath.stem().string()*/}
    , m_filePath{filePath}
{
    m_name = m_filePath.stem().string();
    this->read();
}

intmax_t Scene::geometryModelIndex(GeometryModel* geometryModel) const
{
    auto it = std::find_if(m_geometryModelList.cbegin(), m_geometryModelList.cend(),
            [=](const std::unique_ptr<GeometryModel>& ptr) { return ptr.get() == geometryModel; });
    if (it == m_geometryModelList.cend())
        return -1;
    return std::distance(m_geometryModelList.cbegin(), it);
}

const CoordinateSystemModel* Scene::coordinateSystemByName(const std::string& csysName) const
{
    auto it = std::find_if(m_csysModelList.cbegin(), m_csysModelList.cend(),
            [=](const std::unique_ptr<CoordinateSystemModel>& ptr) { return ptr->name() == csysName; });
    return (it != m_csysModelList.cend()) ? it->get() : nullptr;
}

void Scene::setFilePath(const fs::path& filePath)
{
    m_name = filePath.stem().string();
    m_filePath = filePath;
}

void Scene::setSceneChanged(bool changed)
{
    if (m_sceneChanged == changed)
        return;
    m_sceneChanged = changed;
    sceneChanged.emit_signal();
}

void Scene::read()
{
    CHECK(this->isNamed());

    LOG(INFO) << "Reading the Scene from file.";

    try {
        ptree scenePropTree;

        read_json(m_filePath.string(), scenePropTree);

        ptree coordinateSystemsPropTree = scenePropTree.get_child(kCoordinateSystemsTag);
        this->loadCsysModelList(coordinateSystemsPropTree);

        ptree geometryModelsPropTree = scenePropTree.get_child(kGeometryModelsTag);
        this->loadGeometryModelList(geometryModelsPropTree);

        ptree viewpointsPropTree = scenePropTree.get_child(kViewpointsTag);
        this->loadViewpointList(viewpointsPropTree);
    }
    catch (const std::exception& e) {
        LOG(ERROR) << e.what();
        throw ModelException{fmt::format(ModelException::kFailedToRead, m_filePath.string())};
    }

    this->setSceneChanged(false);
}

void Scene::write()
{
    CHECK(this->isNamed());

    LOG(INFO) << "Writing the Scene to file.";

    try {
        ptree scenePropTree;

        ptree coordinateSystemsPropTree;
        this->saveModelList(m_csysModelList, coordinateSystemsPropTree);
        scenePropTree.add_child(kCoordinateSystemsTag, coordinateSystemsPropTree);

        ptree geometryModelsPropTree;
        this->saveModelList(m_geometryModelList, geometryModelsPropTree);
        scenePropTree.add_child(kGeometryModelsTag, geometryModelsPropTree);

        ptree viewpointsPropTree;
        this->saveModelList(m_viewpointList, viewpointsPropTree);
        scenePropTree.add_child(kViewpointsTag, viewpointsPropTree);

        write_json(m_filePath.string(), scenePropTree);
    }
    catch (const std::exception& e) {
        LOG(ERROR) << e.what();
        throw ModelException{fmt::format(ModelException::kFailedToWrite, m_filePath.string())};
    }

    this->setSceneChanged(false);
}

void Scene::add(std::unique_ptr<CoordinateSystemModel> csysModel)
{
    auto rawPtr = csysModel.get();
    m_csysModelList.push_back(std::move(csysModel));
    coordinateSystemModelAdded.emit_signal(rawPtr); // emit signal

    this->setSceneChanged(true);

    rawPtr->thanuvaModelChanged.connect<Scene, &Scene::handleModelObjectChanged>(this);
}

void Scene::add(std::unique_ptr<GeometryModel> geometryModel)
{
    auto rawPtr = geometryModel.get();
    m_geometryModelList.push_back(std::move(geometryModel));
    geometryModelAdded.emit_signal(rawPtr); // emit signal

    this->setSceneChanged(true);

    rawPtr->thanuvaModelChanged.connect<Scene, &Scene::handleModelObjectChanged>(this);
}

void Scene::add(std::unique_ptr<Viewpoint> viewpoint)
{
    auto rawPtr = viewpoint.get();
    m_viewpointList.push_back(std::move(viewpoint));
    viewpointAdded.emit_signal(rawPtr); // emit signal

    this->setSceneChanged(true);

    rawPtr->thanuvaModelChanged.connect<Scene, &Scene::handleModelObjectChanged>(this);
}

void Scene::loadCsysModelList(const ptree& coordinateSystemsPropTree)
{
    for (const auto& it : coordinateSystemsPropTree) {
        const ptree& modelObjectPropTree = it.second;
        auto csysModel = std::make_unique<CoordinateSystemModel>(this);
        csysModel->load(modelObjectPropTree);
        this->add(std::move(csysModel));
    }
}

void Scene::loadGeometryModelList(const ptree& geometryModelsPropTree)
{
    std::unordered_map<std::string, std::function<std::unique_ptr<GeometryModel>(Scene*)>> geometryModelMakerMap;
    geometryModelMakerMap[BoxModel::kType] = [](Scene* scene) -> std::unique_ptr<GeometryModel> { return std::make_unique<BoxModel>(scene); };
    geometryModelMakerMap[CylinderModel::kType] = [](Scene* scene) -> std::unique_ptr<GeometryModel> { return std::make_unique<CylinderModel>(scene); };
    geometryModelMakerMap[ConeModel::kType] = [](Scene* scene) -> std::unique_ptr<GeometryModel> { return std::make_unique<ConeModel>(scene); };
    geometryModelMakerMap[SphereModel::kType] = [](Scene* scene) -> std::unique_ptr<GeometryModel> { return std::make_unique<SphereModel>(scene); };
    geometryModelMakerMap[MeshModel::kType] = [](Scene* scene) -> std::unique_ptr<GeometryModel> { return std::make_unique<MeshModel>(scene); };

    for (const auto& it : geometryModelsPropTree) {
        const ptree& modelObjectPropTree = it.second;
        std::string type = modelObjectPropTree.get<std::string>(GeometryModel::kTypeTag);
        auto it = geometryModelMakerMap.find(type);
        if (it == geometryModelMakerMap.end())
            throw ModelException{ModelException::kInvalidType};

        auto geometryModel = it->second(this);
        geometryModel->load(modelObjectPropTree);
        this->add(std::move(geometryModel));
    }
}

void Scene::loadViewpointList(const ptree& viewpointsPropTree)
{
    for (const auto& it : viewpointsPropTree) {
        const ptree& modelObjectPropTree = it.second;
        auto viewpoint = std::make_unique<Viewpoint>(this);
        viewpoint->load(modelObjectPropTree);
        this->add(std::move(viewpoint));
    }
}

} // namespace Model
