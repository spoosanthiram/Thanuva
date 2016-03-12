/*
 * Model: Model objects for Thanuva
 *
 * Copyright 2016, Saravanan Poosanthiram
 * All rights reserved.
 */

#include "Stl.h"

#include <boost/property_tree/ptree.hpp>

namespace {

const char* kFilePathTag = "filePath";

} // anonymous

namespace Model {

Stl::Stl(const Project& project, const std::string& filePath)
    : ModelObject{project}
    , m_filePath{filePath}
{
    // TODO: check if STL is exists
}

void Stl::setFilePath(const std::string& filePath, Core::EmitSignal emitSignal)
{
    if (filePath == m_filePath)
        return;

    // TODO: check if STL is exists
    m_filePath = filePath;

    if (Core::EmitSignal::Emit == emitSignal)
        modelObjectChanged.emit_signal(); // emit signal
}

void Stl::loadModel(const boost::property_tree::ptree& modelPropTree)
{
    m_filePath = modelPropTree.get<std::string>(kFilePathTag);
}

void Stl::saveModel(boost::property_tree::ptree& modelPropTree)
{
    modelPropTree.put(kFilePathTag, m_filePath);
}

} // namespace Model
