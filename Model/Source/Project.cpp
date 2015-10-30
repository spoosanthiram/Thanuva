/**
* Author: Saravanan Poosanthiram
* $LastChangedBy: ps $
* $LastChangedDate: 2015-03-20 18:08:01 -0400 (Fri, 20 Mar 2015) $
*/

#include "Project.h"

#include <memory>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

#define GLOG_NO_ABBREVIATED_SEVERITIES
#include <glog/logging.h>

#include "Box.h"
#include "ProjectException.h"
#include "Stl.h"

namespace {

const char* kGeometriesTag = "geometries";
const char* kGeometryTag = "geometry";
const char* kViewpointCameraTag = "viewpointCamera";

} // anonymous

namespace Model {

const char* Project::kDefaultName = "<unsaved>";
const char* Project::kFileExtention = "gfx";

std::string Project::defaultPath()
{
#ifdef WIN32
    const DWORD buflen = 1024;
    WCHAR buf[buflen];
    DWORD len = GetCurrentDirectoryW(buflen, buf);
    std::wstring wpwd{buf};
    std::string pwd{};
    std::transform(wpwd.begin(), wpwd.end(), std::back_inserter(pwd),
            [](const wchar_t wc) { return L'\\' == wc ? '/' : static_cast<char>(wc); });
    return pwd;
#else
    return std::string{std::getenv("PWD")};
#endif
}

Project::Project()
#ifndef _MSC_VER
    : m_name{kDefaultName}
    , m_path{Project::defaultPath()}
#else
    : m_name(kDefaultName)
    , m_path(Project::defaultPath())
#endif
{
}

Project::Project(const std::string& filePath)
    : m_name{}
    , m_path{}
{
    this->setFilePath(filePath);
    this->load();
}

std::string Project::filePath() const
{
    return m_path + "/" + m_name + "." + Project::kFileExtention;
}

/**
 * Assumption: filePath path separator will always be '/'
 * no matter it is Windows or Linux
 */
void Project::setFilePath(const std::string& filePath)
{
    std::string extstr{kFileExtention};
    try {
        if (filePath.substr(filePath.size() - extstr.size()) != extstr) {
            ProjectException e{std::string{ProjectException::kBadExtension} + kFileExtention};
            LOG(ERROR) << e.what();
            throw e;
        }
    }
    catch (std::out_of_range& e) {
        LOG(ERROR) << e.what();
        throw;
    }

    std::size_t pos = filePath.find_last_of('/');
    if (pos != std::string::npos) {
        m_path = filePath.substr(0, pos);
        m_name = filePath.substr(pos + 1, filePath.find_last_of('.') - pos - 1);
    }
    else {
        m_path = "";
        m_name = filePath.substr(0, filePath.find_last_of('.'));
    }
}

void Project::setDirty(bool dirty)
{
    if (dirty == m_dirty)
        return;
    m_dirty = dirty;
    dirtyChanged();
}

void Project::add(const std::shared_ptr<Geometry>& geometry)
{
    LOG(INFO) << "Adding geometry to the list";

    m_geometryList.push_back(geometry);
    geometryAdded(geometry.get()); // emit signal

    this->setDirty(true);
    geometry->geometryChanged.connect<Project, &Project::handleGeometryChanged>(this);
}

void Project::load()
{
    CHECK(this->isNamed());

    LOG(INFO) << "Loading the project";

    using boost::property_tree::ptree;

    try {
        ptree projectPropTree;

        read_json(this->filePath(), projectPropTree);

        ptree geometiesPropTree = projectPropTree.get_child(kGeometriesTag);
        this->loadGeometryList(geometiesPropTree);

        ptree cameraPropTree = projectPropTree.get_child(kViewpointCameraTag);
        m_viewpointCameraModel.load(cameraPropTree);
    }
    catch (std::exception& e) {
        LOG(ERROR) << e.what();
    }

    this->setDirty(false);
}

void Project::save()
{
    CHECK(this->isNamed());

    LOG(INFO) << "Saving the project";

    using boost::property_tree::ptree;

    ptree projectPropTree;

    ptree geometiesPropTree;
    this->saveGeometryList(geometiesPropTree);
    projectPropTree.add_child(kGeometriesTag, geometiesPropTree);

    ptree cameraPropTree;
    m_viewpointCameraModel.save(cameraPropTree);
    projectPropTree.add_child(kViewpointCameraTag, cameraPropTree);

    write_json(this->filePath(), projectPropTree);

    this->setDirty(false);
}

void Project::loadGeometryList(const boost::property_tree::ptree& geometiesPropTree)
{
    using boost::property_tree::ptree;

    for (const auto& it : geometiesPropTree) {
        const ptree& geometryPropTree = it.second;
        auto type = static_cast<Geometry::Type>(geometryPropTree.get<int>(Geometry::kTypeTag));

        std::shared_ptr<Geometry> geometry;

        switch(type) {
        case Geometry::Type::Box:
            geometry = std::make_shared<Model::Box>(*this);
            break;
        case Geometry::Type::Stl:
            geometry = std::make_shared<Model::Stl>(*this);
            break;
        }

        if (geometry) {
            geometry->load(geometryPropTree);
            this->add(geometry);
        }
    }
}

void Project::saveGeometryList(boost::property_tree::ptree& geometiesPropTree)
{
    for (auto& geometry : m_geometryList) {
        boost::property_tree::ptree geometryPropTree;
        geometry->save(geometryPropTree);
        geometiesPropTree.add_child(kGeometryTag, geometryPropTree);
    }
}

} // namespace Model
