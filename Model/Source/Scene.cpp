/*
 * Model: Model objects for Thanuva
 *
 * Copyright 2016, Saravanan Poosanthiram
 * All rights reserved.
 */

#include "Scene.h"

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <fmt/format.h>

#include "BoxModel.h"
#include "CylinderModel.h"
#include "ModelException.h"
#include "StlModel.h"

namespace {

const char* kModelObjectsTag = "modelobjects";
const char* kModelObjectTag = "modelobject";
const char* kViewpointCameraTag = "viewpoint";

} // anonymous

namespace Model {

const char* Scene::kDefaultName = "New Scene";

Scene::Scene(const ThanuvaApp& thanuvaApp)
    : m_thanuvaApp{thanuvaApp}
    , m_name{kDefaultName}
    , m_filePath{}
    , m_modelObjectList{}
{
}

Scene::Scene(const ThanuvaApp& thanuvaApp, const fs::path& filePath)
    : m_thanuvaApp{thanuvaApp}
    , m_name{/*m_filePath.stem().string()*/}
    , m_filePath{filePath}
    , m_modelObjectList{}
{
    m_name = m_filePath.stem().string();
    this->read();
}

intmax_t Scene::index(ModelObject* modelObject)
{
    auto it = std::find_if(m_modelObjectList.cbegin(), m_modelObjectList.cend(),
        [=](const std::unique_ptr<ModelObject>& ptr) { return ptr.get() == modelObject; });
    if (it == m_modelObjectList.cend())
        return -1;
    return std::distance(m_modelObjectList.cbegin(), it);
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
        using boost::property_tree::ptree;
        ptree scenePropTree;

        read_json(m_filePath.string(), scenePropTree);

        ptree modelObjectsPropTree = scenePropTree.get_child(kModelObjectsTag);
        this->loadModelObjectList(modelObjectsPropTree);

        ptree cameraPropTree = scenePropTree.get_child(kViewpointCameraTag);
        m_viewpoint.load(cameraPropTree);
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
        using boost::property_tree::ptree;
        ptree scenePropTree;

        ptree modelObjectsPropTree;
        this->saveModelObjectList(modelObjectsPropTree);
        scenePropTree.add_child(kModelObjectsTag, modelObjectsPropTree);

        ptree cameraPropTree;
        m_viewpoint.save(cameraPropTree);
        scenePropTree.add_child(kViewpointCameraTag, cameraPropTree);

        write_json(m_filePath.string(), scenePropTree);
    }
    catch (const std::exception& e) {
        LOG(ERROR) << e.what();
        throw ModelException{fmt::format(ModelException::kFailedToWrite, m_filePath.string())};
    }

    this->setSceneChanged(false);
}

void Scene::add(std::unique_ptr<ModelObject> modelObjectPtr)
{
    ModelObject* modelObject = modelObjectPtr.get();
    m_modelObjectList.push_back(std::move(modelObjectPtr));
    modelObjectAdded.emit_signal(modelObject); // emit signal

    this->setSceneChanged(true);

    modelObject->modelObjectChanged.connect<Scene, &Scene::handleModelObjectChanged>(this);
}

void Scene::loadModelObjectList(const boost::property_tree::ptree& modelObjectsPropTree)
{
    using boost::property_tree::ptree;

    std::function<std::unique_ptr<ModelObject>(Scene*)> modelObjectMaker[] = {
        [](Scene* scene) -> std::unique_ptr<ModelObject> { return std::make_unique<BoxModel>(*scene); },
        [](Scene* scene) -> std::unique_ptr<ModelObject> { return std::make_unique<StlModel>(*scene); },
        [](Scene* scene) -> std::unique_ptr<ModelObject> { return std::make_unique<CylinderModel>(*scene); }
    };

    for (const auto& it : modelObjectsPropTree) {
        const ptree& modelObjectPropTree = it.second;
        unsigned int type = modelObjectPropTree.get<unsigned int>(ModelObject::kTypeTag);
        if (type > static_cast<unsigned int>(ModelObject::Type::NTypes))
            throw ModelException{ModelException::kInvalidType};

        auto modelObject = modelObjectMaker[type](this);

        if (modelObject) {
            modelObject->load(modelObjectPropTree);
            this->add(std::move(modelObject));
        }
    }
}

void Scene::saveModelObjectList(boost::property_tree::ptree& modelObjectsPropTree)
{
    for (auto& modelObject : m_modelObjectList) {
        boost::property_tree::ptree modelObjectPropTree;
        modelObject->save(modelObjectPropTree);
        modelObjectsPropTree.add_child(kModelObjectTag, modelObjectPropTree);
    }
}

} // namespace Model
