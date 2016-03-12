/*
 * Model: Model objects for Thanuva
 *
 * Copyright 2016, Saravanan Poosanthiram
 * All rights reserved.
 */

#include "ModelObject.h"

#include <boost/property_tree/ptree.hpp>

namespace {

const char* kMaterialTag = "material";
const char* kTransformMatrixTag = "transformMatrix";

} // anonymous

namespace Model {

const char* ModelObject::kTypeTag = "type";

void ModelObject::setMaterial(const Core::Material& material, Core::EmitSignal emitSignal)
{
    if (material == m_material)
        return;

    m_material = material;
    if (Core::EmitSignal::Emit == emitSignal)
        materialChanged.emit_signal(); // emit signal
}

void ModelObject::setTransformMatrix(const Core::Matrix4x4& transformMatrix, Core::EmitSignal emitSignal)
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
