/*
 * Model: Model objects for Thanuva
 *
 * Copyright 2016, Saravanan Poosanthiram
 * All rights reserved.
 */

#include "ModelObject.h"

#include <boost/property_tree/ptree.hpp>
#include <fmt/format.h>
#include <glog/logging.h>

#include "ModelException.h"
#include "Utils.h"

namespace {

const char* kMaterialTag = "material";
const char* kTransformMatrixTag = "transformMatrix";

} // anonymous

namespace Model {

const char* ModelObject::kTypeTag = "type";

std::string ModelObject::typeStr(Type type)
{
    std::string str{};

    switch (type)
    {
        case Type::Box:
            str = "Box";
            break;
        case Type::Stl:
            str = "Stl";
            break;
    }

    return str;
}

ModelObject::ModelObject(const Scene& scene)
    : m_scene{scene}
{
    nameChanged.connect<ModelObject, &ModelObject::emitModelObjectChanged>(this);
    materialChanged.connect<ModelObject, &ModelObject::emitModelObjectChanged>(this);
    transformMatrixChanged.connect<ModelObject, &ModelObject::emitModelObjectChanged>(this);
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

    if (newName == m_name)
        return;

    m_name = newName;
    nameChanged.emit_signal(); // emit signal
}

void ModelObject::setMaterial(const Core::Material& material)
{
    if (material == m_material)
        return;

    m_material = material;
    materialChanged.emit_signal(); // emit signal
}

void ModelObject::setTransformMatrix(const Core::Matrix4x4& transformMatrix,
                                     Core::EmitSignal emitSignal)
{
    if (transformMatrix == m_transformMatrix)
        return;

    m_transformMatrix = transformMatrix;
    if (Core::EmitSignal::Emit == emitSignal)
        transformMatrixChanged.emit_signal(); // emit signal
}

void ModelObject::load(const boost::property_tree::ptree& modelPropTree)
{
    m_material.set(modelPropTree.get<std::string>(kMaterialTag));
    m_transformMatrix.set(modelPropTree.get<std::string>(kTransformMatrixTag));

    this->loadModel(modelPropTree);
}

void ModelObject::save(boost::property_tree::ptree& modelPropTree)
{
    modelPropTree.put(kTypeTag, static_cast<int>(this->type()));
    modelPropTree.put(kMaterialTag, m_material.str());
    modelPropTree.put(kTransformMatrixTag, m_transformMatrix.str());

    this->saveModel(modelPropTree);
}

} // namespace Model
