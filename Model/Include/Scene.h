/*
 * Model: Model objects for Thanuva
 *
 * Copyright 2016, Saravanan Poosanthiram
 * All rights reserved.
 */

#ifndef Model_Scene_h
#define Model_Scene_h

#include <filesystem>
#include <memory>
#include <string>
#include <vector>

#include <boost/property_tree/ptree_fwd.hpp>
#include <glog/logging.h>
#include <nano_signal_slot.hpp>

#include "CoordinateSystemModel.h"
#include "GeometryModel.h"
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
    const std::vector<std::unique_ptr<CoordinateSystemModel>>& coordinateSystemModelList() const
    {
        return m_csysModelList;
    }
    const std::vector<std::unique_ptr<GeometryModel>>& geometryModelList() const { return m_geometryModelList; }
    const std::vector<std::unique_ptr<Viewpoint>>& viewpointList() const { return m_viewpointList; }

    intmax_t geometryModelIndex(GeometryModel* geometryModel) const;
    const CoordinateSystemModel* coordinateSystemByName(const std::string& csysName) const;

    void setFilePath(const fs::path& filePath);
    void setSceneChanged(bool changed);

    template<typename ModelObjectType, typename ...Args> ModelObjectType* newModelObject(Args&&... args)
    {
        LOG(INFO) << "Create ThanuvaModel and Add to the list";

        std::unique_ptr<ModelObjectType> modelObjectPtr = std::make_unique<ModelObjectType>(this, std::forward<Args>(args)...);
        ModelObjectType* modelObject = modelObjectPtr.get();
        this->add(std::move(modelObjectPtr));
        return modelObject;
    }
    void read();
    void write();

public: // signals
    Nano::Signal<void()> sceneChanged{};
    Nano::Signal<void(CoordinateSystemModel*)> coordinateSystemModelAdded{};
    Nano::Signal<void(GeometryModel*)> geometryModelAdded{};
    Nano::Signal<void(Viewpoint*)> viewpointAdded{};

protected: // slots
    void handleModelObjectChanged(Model::ThanuvaModel* /*thanuvaModel*/) { this->setSceneChanged(true); }

private:
    void add(std::unique_ptr<CoordinateSystemModel> csysModel);
    void add(std::unique_ptr<GeometryModel> geometryModel);
    void add(std::unique_ptr<Viewpoint> viewpoint);

    void loadCsysModelList(const boost::property_tree::ptree& coordinateSystemsPropTree);
    void loadGeometryModelList(const boost::property_tree::ptree& geometryModelsPropTree);
    void loadViewpointList(const boost::property_tree::ptree& viewpointsPropTree);

    template<typename ModelListType>
    void saveModelList(const ModelListType& modelList, boost::property_tree::ptree& modelsPropTree)
    {
        for (auto& model : modelList) {
            ptree modelObjectPropTree;
            model->save(modelObjectPropTree);
            modelsPropTree.add_child(kModelObjectTag, modelObjectPropTree);
        }
    }

    const ThanuvaApp& m_thanuvaApp;
    std::string m_name;
    fs::path m_filePath;
    bool m_sceneChanged{false};
    std::vector<std::unique_ptr<CoordinateSystemModel>> m_csysModelList{};
    std::vector<std::unique_ptr<GeometryModel>> m_geometryModelList{};
    std::vector<std::unique_ptr<Viewpoint>> m_viewpointList{};
};

} // namespace Model

#endif // Model_Scene_h
