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

const char* kEyeRotationMatrixTag = "eyeRotationMatrix";
const char* kZoomLevelTag = "zoomLevel";

} // anonymous

namespace Model {

void Viewpoint::load(const boost::property_tree::ptree& cameraPropTree)
{
    m_eyeRotationMatrix.set(cameraPropTree.get<std::string>(kEyeRotationMatrixTag));
    m_zoomLevel = cameraPropTree.get<double>(kZoomLevelTag);
}

void Viewpoint::save(boost::property_tree::ptree& cameraPropTree)
{
    cameraPropTree.put(kEyeRotationMatrixTag, m_eyeRotationMatrix.str());
    cameraPropTree.put(kZoomLevelTag, m_zoomLevel);
}

} // namespace Model
