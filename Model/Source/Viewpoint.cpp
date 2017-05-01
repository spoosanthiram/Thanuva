/*
 * Model: Model objects for Thanuva
 *
 * Copyright 2016, Saravanan Poosanthiram
 * All rights reserved.
 */

#include "Viewpoint.h"

#include <boost/property_tree/ptree.hpp>

#include "Vector3d.h"

namespace {

const char* kZoomLevelTag = "zoomLevel";
const char* kEyeRotationMatrixTag = "eyeRotationMatrix";
const char* kTranslationTag = "translation";

} // anonymous

namespace Model {

Viewpoint::Viewpoint(const Scene* scene)
    : ThanuvaModel{scene}
{
    zoomLevelChanged.connect<ThanuvaModel, &ThanuvaModel::emitThanuvaModelChanged>(this);
    eyeRotationMatrixChanged.connect<ThanuvaModel, &ThanuvaModel::emitThanuvaModelChanged>(this);
    translationChanged.connect<ThanuvaModel, &ThanuvaModel::emitThanuvaModelChanged>(this);
}

void Viewpoint::setZoomLevel(double zoomLevel)
{
    if (m_zoomLevel != zoomLevel) {
        m_zoomLevel = zoomLevel;
        zoomLevelChanged.emit_signal(); // emit signal so the scene can become changed and so to be saved.
    }
}

void Viewpoint::setEyeRotationMatrix(const Core::Matrix3x3& eyeRotationMatrix)
{
    m_eyeRotationMatrix = eyeRotationMatrix;
    eyeRotationMatrixChanged.emit_signal(); // emit signal so the scene can become changed and so to be saved.
}

void Viewpoint::setTranslation(const Core::Point3d& translation)
{
    m_translation = translation;
    translationChanged.emit_signal(); // // emit signal so the scene can become changed and so to be saved.
}

void Viewpoint::loadModel(const boost::property_tree::ptree& modelPropTree)
{
    m_zoomLevel = modelPropTree.get<double>(kZoomLevelTag);
    m_eyeRotationMatrix.set(modelPropTree.get<std::string>(kEyeRotationMatrixTag));
    m_translation.set(modelPropTree.get<std::string>(kTranslationTag));
}

void Viewpoint::saveModel(boost::property_tree::ptree& modelPropTree)
{
    modelPropTree.put(kEyeRotationMatrixTag, m_eyeRotationMatrix.str());
    modelPropTree.put(kZoomLevelTag, m_zoomLevel);
    modelPropTree.put(kTranslationTag, m_translation.str());
}

} // namespace Model
