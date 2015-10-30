/**
* Author: Saravanan Poosanthiram
* $LastChangedBy: ps $
* $LastChangedDate: 2015-03-20 18:08:01 -0400 (Fri, 20 Mar 2015) $
*/

#ifndef MODEL_PROJECT_H
#define MODEL_PROJECT_H

#include <memory>
#include <vector>

#include "ViewpointCameraModel.h"

#include <boost/property_tree/ptree_fwd.hpp>

#include "nano_signal_slot.h"

namespace Model {

class Geometry;

class Project {
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
    const std::vector<std::shared_ptr<Geometry>>& geometryList() const { return m_geometryList; }
    const ViewpointCameraModel& viewpointCameraModel() const { return m_viewpointCameraModel; }
    ViewpointCameraModel& viewpointCameraModel() { return m_viewpointCameraModel; }

    void setFilePath(const std::string& filePath);
    void setDirty(bool dirty);

    void add(const std::shared_ptr<Geometry>& geometry);
    void load();
    void save();

public:
    Nano::Signal<void()> dirtyChanged;
    Nano::Signal<void(Geometry*)> geometryAdded;

protected: // slots
    void handleGeometryChanged() { this->setDirty(true); }

private:
    void loadGeometryList(const boost::property_tree::ptree& geometiesPropTree);
    void saveGeometryList(boost::property_tree::ptree& geometiesPropTree);

    std::string m_name;
    std::string m_path;
    bool m_dirty{false};
    std::vector<std::shared_ptr<Geometry>> m_geometryList{};
    ViewpointCameraModel m_viewpointCameraModel{};
};

} // namespace Model

#endif // MODEL_PROJECT_H
