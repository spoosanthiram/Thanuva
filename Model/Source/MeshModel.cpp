/*
 * Model: Model objects for Thanuva
 *
 * Copyright 2016, Saravanan Poosanthiram
 * All rights reserved.
 */

#include "MeshModel.h"

#include <boost/property_tree/ptree.hpp>
#include <fmt/format.h>
#include <glog/logging.h>

#include "ModelException.h"

namespace {

const char* kFilePathTag = "filePath";

} // anonymous

namespace Model {

const char* MeshModel::kType = "Mesh";

MeshModel::MeshModel(const Scene* scene)
    : GeometryModel{scene}
{
}

MeshModel::MeshModel(const Scene* scene, const fs::path& filePath)
    : GeometryModel{scene}
    , m_filePath{filePath}
{
    if (!fs::exists(m_filePath)) {
        ModelException e{fmt::format(ModelException::kPathNotFound, m_filePath.string())};
        LOG(ERROR) << e.what();
        throw e;
    }

    filePathChanged.connect<ThanuvaModel, &ThanuvaModel::emitThanuvaModelChanged>(this);
}

void MeshModel::setFilePath(const fs::path& filePath, Core::EmitSignal emitSignal)
{
    if (m_filePath == filePath)
        return;

    if (!fs::exists(filePath)) {
        ModelException e{fmt::format(ModelException::kPathNotFound, filePath.string())};
        LOG(ERROR) << e.what();
        throw e;
    }
    m_filePath = filePath;

    if (Core::EmitSignal::Emit == emitSignal)
        filePathChanged.emit_signal(); // emit signal
}

void MeshModel::loadGeometryModel(const boost::property_tree::ptree& modelPropTree)
{
    m_filePath = modelPropTree.get<std::string>(kFilePathTag);
}

void MeshModel::saveGeometryModel(boost::property_tree::ptree& modelPropTree)
{
    modelPropTree.put(kFilePathTag, m_filePath);
}

} // namespace Model
