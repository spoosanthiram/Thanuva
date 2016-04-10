/*
 * Model: Model objects for Thanuva
 *
 * Copyright 2016, Saravanan Poosanthiram
 * All rights reserved.
 */

#include "Project.h"

#include <memory>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <cppformat/format.h>
#include <glog/logging.h>

#include "BoxModel.h"
#include "ModelException.h"
#include "StlModel.h"

namespace {

const char* kModelObjectsTag = "modelobjects";
const char* kModelObjectTag = "modelobject";
const char* kViewpointCameraTag = "viewpoint";

} // anonymous

namespace Model {

const char* Project::kDefaultName = "Untitled";
const char* Project::kFileExtention = "tha";

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
    : m_name{kDefaultName}
    , m_path{Project::defaultPath()}
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
            ModelException e{fmt::format(ModelException::kBadExtension, filePath, kFileExtention)};
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
    dirtyChanged.emit_signal();
}

void Project::add(const std::shared_ptr<ModelObject>& modelObject)
{
    LOG(INFO) << "Adding ModelObject to the list";

    m_modelObjectList.push_back(modelObject);
    modelObjectAdded.emit_signal(modelObject.get()); // emit signal

    this->setDirty(true);
    modelObject->modelObjectChanged.connect<Project, &Project::handleModelObjectChanged>(this);
}

void Project::load()
{
    CHECK(this->isNamed());

    LOG(INFO) << "Loading the project";

    using boost::property_tree::ptree;

    try {
        ptree projectPropTree;

        read_json(this->filePath(), projectPropTree);

        ptree modelObjectsPropTree = projectPropTree.get_child(kModelObjectsTag);
        this->loadModelObjectList(modelObjectsPropTree);

        ptree cameraPropTree = projectPropTree.get_child(kViewpointCameraTag);
        m_viewpoint.load(cameraPropTree);
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

    ptree modelObjectsPropTree;
    this->saveModelObjectList(modelObjectsPropTree);
    projectPropTree.add_child(kModelObjectsTag, modelObjectsPropTree);

    ptree cameraPropTree;
    m_viewpoint.save(cameraPropTree);
    projectPropTree.add_child(kViewpointCameraTag, cameraPropTree);

    write_json(this->filePath(), projectPropTree);

    this->setDirty(false);
}

void Project::loadModelObjectList(const boost::property_tree::ptree& modelObjectsPropTree)
{
    using boost::property_tree::ptree;

    for (const auto& it : modelObjectsPropTree) {
        const ptree& modelObjectPropTree = it.second;
        auto type = static_cast<ModelObject::Type>(
                        modelObjectPropTree.get<int>(ModelObject::kTypeTag));

        std::shared_ptr<ModelObject> modelObject;

        switch(type) {
        case ModelObject::Type::Box:
            modelObject = std::make_shared<BoxModel>(*this);
            break;
        case ModelObject::Type::Stl:
            modelObject = std::make_shared<StlModel>(*this);
            break;
        }

        if (modelObject) {
            modelObject->load(modelObjectPropTree);
            this->add(modelObject);
        }
    }
}

void Project::saveModelObjectList(boost::property_tree::ptree& modelObjectsPropTree)
{
    for (auto& modelObject : m_modelObjectList) {
        boost::property_tree::ptree modelObjectPropTree;
        modelObject->save(modelObjectPropTree);
        modelObjectsPropTree.add_child(kModelObjectTag, modelObjectPropTree);
    }
}

} // namespace Model
