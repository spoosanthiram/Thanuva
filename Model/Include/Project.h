/*
 * Model: Model objects for Thanuva
 *
 * Copyright 2016, Saravanan Poosanthiram
 * All rights reserved.
 */

#ifndef MODEL_PROJECT_H
#define MODEL_PROJECT_H

#include <memory>
#include <vector>

#include <boost/property_tree/ptree_fwd.hpp>
#include <nano_signal_slot.hpp>

#include "Viewpoint.h"

namespace Model {

class ModelObject;

class Project
{
public:
    static std::string defaultPath();

    static const char* kDefaultName;
    static const char* kFileExtention;

public:
    Project();
    Project(const std::string& filePath);

    const std::string& name() const { return m_name; }
    bool isNamed() const { return m_name != kDefaultName; }
    const std::string& path() const { return m_path; }
    std::string filePath() const;
    bool isDirty() const { return m_dirty; }
    const std::vector<std::shared_ptr<ModelObject>>& modelObjectList() const { return m_modelObjectList; }
    const Viewpoint& viewpoint() const { return m_viewpoint; }

    void setFilePath(const std::string& filePath);
    void setDirty(bool dirty);

    void add(const std::shared_ptr<ModelObject>& modelObject);
    void load();
    void save();

public:
    Nano::Signal<void()> dirtyChanged{};
    Nano::Signal<void(ModelObject*)> modelObjectAdded{};

protected: // slots
    void handleModelObjectChanged() { this->setDirty(true); }

private:
    void loadModelObjectList(const boost::property_tree::ptree& modelObjectsPropTree);
    void saveModelObjectList(boost::property_tree::ptree& modelObjectsPropTree);

    std::string m_name;
    std::string m_path;
    bool m_dirty{false};
    std::vector<std::shared_ptr<ModelObject>> m_modelObjectList{};
    Viewpoint m_viewpoint{};
};

} // namespace Model

#endif // MODEL_PROJECT_H
