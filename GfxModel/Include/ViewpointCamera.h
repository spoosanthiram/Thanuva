/**
* Author: Saravanan Poosanthiram
* $LastChangedBy: ps $
* $LastChangedDate: 2015-03-20 18:08:01 -0400 (Fri, 20 Mar 2015) $
*/

#ifndef GFXMODEL_VIEWPOINTCAMERA_H
#define GFXMODEL_VIEWPOINTCAMERA_H

#include "Matrix4x4.h"
#include "ViewpointCameraModel.h"

#include "nano_signal_slot.h"

namespace GfxModel {

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

} // namespace GfxModel

#endif // GFXMODEL_VIEWPOINTCAMERA_H
