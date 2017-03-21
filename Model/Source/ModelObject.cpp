/*
 * Model: Model objects for Thanuva
 *
 * Copyright 2016, Saravanan Poosanthiram
 * All rights reserved.
 */

#include "ModelObject.h"

#include <regex>

#include <boost/property_tree/ptree.hpp>
#include <fmt/format.h>
#include <glog/logging.h>

#include "CoreDef.h"
#include "ModelException.h"
#include "Utils.h"

namespace {

const char* kNameTag = "name";
const char* kMaterialTag = "material";
const char* kTransformTag = "transform";

} // anonymous

namespace Model {

const char* ModelObject::kTypeTag = "type";

std::string ModelObject::typeStr(Type type)
{
    static const char* typeString[] = {"Box", "Mesh", "Cylinder", "Cone", "Sphere"};

    int index = static_cast<int>(type);
    CHECK(index < sizeof(typeString) / sizeof(const char*));
    return typeString[index];
}

std::string ModelObject::Transform::str() const
{
    return fmt::format("{} {} {}", translateX, translateY, translateZ);
}

void ModelObject::Transform::set(const std::string& str)
{
    std::regex re{Core::kWhitespaceRegEx};
    auto it = std::sregex_token_iterator(str.begin(), str.end(), re, -1);
    translateX = std::stod((*it++).str());
    translateY = std::stod((*it++).str());
    translateZ = std::stod((*it).str());
}

ModelObject::ModelObject(const Scene* scene)
    : m_scene{scene}
{
    nameChanged.connect<ModelObject, &ModelObject::emitModelObjectChanged>(this);
    materialChanged.connect<ModelObject, &ModelObject::emitModelObjectChanged>(this);
    transformChanged.connect<ModelObject, &ModelObject::emitModelObjectChanged>(this);
}

std::string ModelObject::label() const
{
    return fmt::format("{}: {}", this->typeStr(this->type()), m_name);
}

void ModelObject::setName(const std::string& name)
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

void ModelObject::setMaterial(const Core::Material& material)
{
    if (m_material == material)
        return;

    m_material = material;
    materialChanged.emit_signal(); // emit signal
}

void ModelObject::setTransform(const Transform& transform)
{
    if (m_transform == transform)
        return;

    m_transform = transform;
    transformChanged.emit_signal(); // emit signal
}

void ModelObject::load(const boost::property_tree::ptree& modelPropTree)
{
    m_name = modelPropTree.get<std::string>(kNameTag);
    m_material.set(modelPropTree.get<std::string>(kMaterialTag));
    m_transform.set(modelPropTree.get<std::string>(kTransformTag));

    this->loadModel(modelPropTree);
}

void ModelObject::save(boost::property_tree::ptree& modelPropTree)
{
    modelPropTree.put(kTypeTag, static_cast<int>(this->type()));
    modelPropTree.put(kNameTag, m_name);
    modelPropTree.put(kMaterialTag, m_material.str());
    modelPropTree.put(kTransformTag, m_transform.str());

    this->saveModel(modelPropTree);
}

} // namespace Model
