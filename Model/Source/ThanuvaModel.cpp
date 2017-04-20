/*
 * Model: Model objects for Thanuva
 *
 * Copyright 2016, Saravanan Poosanthiram
 * All rights reserved.
 */

#include "ThanuvaModel.h"

#include <regex>

#include <boost/property_tree/ptree.hpp>
#include <glog/logging.h>

#include "CoreDef.h"
#include "ModelException.h"
#include "Utils.h"

namespace {

const char* kNameTag = "name";

} // anonymous namespace

namespace Model {

ThanuvaModel::ThanuvaModel(const Scene* scene)
    : m_scene{scene}
{
    nameChanged.connect<ThanuvaModel, &ThanuvaModel::emitThanuvaModelChanged>(this);
}

void ThanuvaModel::setName(const std::string& name)
{
    std::string newName = Core::trim(name);
    if (newName.empty()) {
        ModelException e{ModelException::kEmptyName};
        LOG(ERROR) << e.what();
        throw e;
    }

    if (m_name == newName)
        return;

    m_name = newName;
    nameChanged.emit_signal(); // emit signal
}

void ThanuvaModel::load(const boost::property_tree::ptree& modelPropTree)
{
    m_name = modelPropTree.get<std::string>(kNameTag);
    this->loadModel(modelPropTree);
}

void ThanuvaModel::save(boost::property_tree::ptree& modelPropTree)
{
    modelPropTree.put(kNameTag, m_name);
    this->saveModel(modelPropTree);
}

} // namespace Model
