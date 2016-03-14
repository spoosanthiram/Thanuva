/**
* Author: Saravanan Poosanthiram
* $LastChangedBy: ps $
* $LastChangedDate: 2015-03-20 18:08:01 -0400 (Fri, 20 Mar 2015) $
*/

#include "ViewpointCamera.h"

#include "Vector3d.h"

namespace GfxModel {

ViewpointCamera::ViewpointCamera(const GfxProject& gfxProject, Model::ViewpointCameraModel& viewpointCameraModel)
    : m_gfxProject{gfxProject}
    , m_viewpointCameraModel{viewpointCameraModel}
    , m_viewMatrix{Core::Matrix4x4::identity()}
{
    m_viewpointCameraModel.rotationChanged.connect<ViewpointCamera, &ViewpointCamera::updateViewMatrix>(this);
    m_viewpointCameraModel.zoomChanged.connect<ViewpointCamera, &ViewpointCamera::updateViewMatrix>(this);
}

ViewpointCamera::~ViewpointCamera()
{
    m_viewpointCameraModel.rotationChanged.disconnect<ViewpointCamera, &ViewpointCamera::updateViewMatrix>(this);
    m_viewpointCameraModel.zoomChanged.disconnect<ViewpointCamera, &ViewpointCamera::updateViewMatrix>(this);
}

void ViewpointCamera::updateViewMatrix()
{
    Core::Vector3d eye = m_viewpointCameraModel.eyeRotationMatrix()
        * Core::Vector3d{0.0, 0.0, m_viewpointCameraModel.viewpointTranslation()};
    eye.scale(m_viewpointCameraModel.zoomLevel());

    Core::Vector3d w = eye;
    w.normalize();

    Core::Vector3d upVector = m_viewpointCameraModel.eyeRotationMatrix() * Core::Vector3d{0.0, 1.0, 0.0};

    Core::Vector3d u = upVector.cross(w);
    u.normalize();

    Core::Vector3d v = w.cross(u);

    Core::Vector3d negativeEye{-eye.x(), -eye.y(), -eye.z()};

    m_viewMatrix = Core::Matrix4x4{u.x(), v.x(), w.x(), 0.0
        , u.y(), v.y(), w.y(), 0.0
        , u.z(), v.z(), w.z(), 0.0
        , negativeEye.dot(u), negativeEye.dot(v), negativeEye.dot(w), 1.0};

    viewpointCameraChanged.emit_signal(); // emit signal
}

} // namespace GfxModel
