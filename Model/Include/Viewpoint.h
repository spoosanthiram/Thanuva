/*
 * Model: Model objects for Thanuva
 *
 * Copyright 2016, Saravanan Poosanthiram
 * All rights reserved.
 */

#ifndef MODEL_VIEWPOINT_H
#define MODEL_VIEWPOINT_H

#include <boost/property_tree/ptree_fwd.hpp>

#include "Matrix3x3.h"

namespace Model {

class Viewpoint
{
public:
    Viewpoint() : m_eyeRotationMatrix{Core::Matrix3x3::identity()} {}

    const Core::Matrix3x3& eyeRotationMatrix() const { return m_eyeRotationMatrix; }
    Core::Matrix3x3& eyeRotationMatrix() { return m_eyeRotationMatrix; }
    double zoomLevel() const { return m_zoomLevel; }

    void setEyeRotationMatrix(const Core::Matrix3x3& eyeRotationMatrix)
    {
        m_eyeRotationMatrix = eyeRotationMatrix;
    }
    void setZoomLevel(double zoomLevel) { m_zoomLevel = zoomLevel; }

    void load(const boost::property_tree::ptree& cameraPropTree);
    void save(boost::property_tree::ptree& cameraPropTree);

private:
    Core::Matrix3x3 m_eyeRotationMatrix{};
    double m_zoomLevel{1.0};
};

} // namespace Model

#endif // MODEL_VIEWPOINT_H
