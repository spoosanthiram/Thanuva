/*
 * Model: Model objects for Thanuva
 *
 * Copyright 2016, Saravanan Poosanthiram
 * All rights reserved.
 */

#include "GeometryModel.h"

#include <boost/property_tree/ptree.hpp>
#include <fmt/format.h>

#include "Scene.h"

namespace {

const char* kCoordinateSystemTag = "coordinateSystem";
const char* kMaterialTag = "material";

} // anonymous namespace

namespace Model {

const char* GeometryModel::kTypeTag = "type";

GeometryModel::GeometryModel(const Scene* scene)
    : ThanuvaModel{scene}
    , m_coordinateSystemModel{scene->coordinateSystemModelList().front().get()}
{
    coordinateSystemModelChanged.connect<ThanuvaModel, &ThanuvaModel::emitThanuvaModelChanged>(this);
    materialChanged.connect<ThanuvaModel, &ThanuvaModel::emitThanuvaModelChanged>(this);
}

std::string GeometryModel::label() const
{
    return fmt::format("{}: {}", this->type(), this->name());
}

void GeometryModel::setCoordinateSystemModel(const CoordinateSystemModel* csys)
{
    if (m_coordinateSystemModel == csys)
        return;

    m_coordinateSystemModel = csys;
    coordinateSystemModelChanged.emit_signal(); // emit signal
}

void GeometryModel::setMaterial(const Core::Material& material)
{
    if (m_material == material)
        return;

    m_material = material;
    materialChanged.emit_signal(); // emit signal
}

void GeometryModel::loadModel(const boost::property_tree::ptree& modelPropTree)
{
    std::string csysName = modelPropTree.get<std::string>(kCoordinateSystemTag);
    const CoordinateSystemModel* csys  = this->scene()->coordinateSystemByName(csysName);
    CHECK(csys);
    m_coordinateSystemModel = csys;
    m_material.set(modelPropTree.get<std::string>(kMaterialTag));

    this->loadGeometryModel(modelPropTree);
}

void GeometryModel::saveModel(boost::property_tree::ptree& modelPropTree)
{
    modelPropTree.put(kTypeTag, this->type());
    modelPropTree.put(kCoordinateSystemTag, m_coordinateSystemModel->name());
    modelPropTree.put(kMaterialTag, m_material.str());

    this->saveGeometryModel(modelPropTree);
}

} // namespace Model
