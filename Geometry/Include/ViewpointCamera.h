/*
 * Geometry: Geometry objects for Thanuva
 *
 * Copyright 2016, Saravanan Poosanthiram
 * All rights reserved.
 */

#ifndef GEOMETRY_VIEWPOINTCAMERA_H
#define GEOMETRY_VIEWPOINTCAMERA_H

#include "Matrix4x4.h"
#include "ViewpointCameraModel.h"

#include "nano_signal_slot.hpp"

namespace Geometry {

class GfxProject;

class ViewpointCamera : public Nano::Observer {
public:
    ViewpointCamera(const GfxProject& gfxProject, Model::ViewpointCameraModel& viewpointCameraModel);
    ~ViewpointCamera();

    const Core::Matrix4x4& viewMatrix() const { return m_viewMatrix; }

public: // signals
    Nano::Signal<void()> viewpointCameraChanged;

protected: // slots
    void updateViewMatrix();

private:
    const GfxProject& m_gfxProject;
    Model::ViewpointCameraModel& m_viewpointCameraModel;

    Core::Matrix4x4 m_viewMatrix;
};

} // namespace Geometry

#endif // GEOMETRY_VIEWPOINTCAMERA_H
