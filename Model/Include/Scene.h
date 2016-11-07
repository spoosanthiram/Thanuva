/*
 * Model: Model objects for Thanuva
 *
 * Copyright 2016, Saravanan Poosanthiram
 * All rights reserved.
 */

#ifndef MODEL_SCENE_H
#define MODEL_SCENE_H

#include <filesystem>
#include <memory>
#include <string>
#include <vector>

#include <boost/property_tree/ptree_fwd.hpp>
#include <glog/logging.h>
#include <nano_signal_slot.hpp>

#include "ModelObject.h"
#include "Viewpoint.h"

namespace fs = std::experimental::filesystem;

namespace Model {

class ThanuvaApp;

class Scene
{
public:
    static const char* kDefaultName;

public:
    Scene(const ThanuvaApp& thanuvaApp);
    Scene(const ThanuvaApp& thanuvaApp, const fs::path& filePath);
    ~Scene() {}

    const ThanuvaApp& thanuvaApp() const { return m_thanuvaApp; }
    const std::string& name() const { return m_name; }
    bool isNamed() const { return m_name != kDefaultName; }
    fs::path filePath() const { return m_filePath; }
    bool isSceneChanged() const { return m_sceneChanged; }
    /**
     * Returns the underlining vector of unique_ptrs. User of the method should keep a const reference and not try to take a copy
     */
    const std::vector<std::unique_ptr<ModelObject>>& modelObjectList() const { return m_modelObjectList; }
    const Viewpoint& viewpoint() const { return m_viewpoint; }

    void setFilePath(const fs::path& filePath);
    void setSceneChanged(bool changed);

    template<typename ModelObjectType, typename ...Args> ModelObjectType* newModelObject(Args&&... args)
    {
        LOG(INFO) << "Create ModelObject and Add to the list";

        std::unique_ptr<ModelObjectType> modelObjectPtr = std::make_unique<ModelObjectType>(*this, std::forward<Args>(args)...);
        ModelObjectType* modelObject = modelObjectPtr.get();
        this->add(std::move(modelObjectPtr));
        return modelObject;
    }
    void read();
    void write();

public:
    Nano::Signal<void()> sceneChanged{};
    Nano::Signal<void(ModelObject*)> modelObjectAdded{};

protected: // slots
    void handleModelObjectChanged() { this->setSceneChanged(true); }

private:
    void add(std::unique_ptr<ModelObject> modelObjectPtr);
    void loadModelObjectList(const boost::property_tree::ptree& modelObjectsPropTree);
    void saveModelObjectList(boost::property_tree::ptree& modelObjectsPropTree);

    const ThanuvaApp& m_thanuvaApp;
    std::string m_name;
    fs::path m_filePath;
    bool m_sceneChanged{false};
    std::vector<std::unique_ptr<ModelObject>> m_modelObjectList;
    Viewpoint m_viewpoint{};
};

} // namespace Model

#endif // MODEL_SCENE_H
