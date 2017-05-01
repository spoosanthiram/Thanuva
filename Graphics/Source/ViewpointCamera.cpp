/*
 * Graphics: Graphical objects for Thanuva Model/Geometry obejcts
 *
 * Copyright 2016, Saravanan Poosanthiram
 * All rights reserved.
 */

#include "ViewpointCamera.h"

#include "Quaternion.h"
#include "Vector3d.h"

namespace Graphics {

const double ViewpointCamera::kZoomIncrement = 0.02;

ViewpointCamera::ViewpointCamera(const AbstractGraphicsScene& graphicsScene)
    : m_graphicsScene{graphicsScene}
    , m_translationMatrix{Core::Matrix4x4::identity()}
    , m_viewMatrix{Core::Matrix4x4::identity()}
{
}

void ViewpointCamera::zoom(int steps)
{
    m_viewpoint->setZoomLevel(m_viewpoint->zoomLevel() + steps * kZoomIncrement);
    this->updateViewMatrix();
}

void ViewpointCamera::rotate(const Location& startLocation, const Location& endLocation)
{
    Core::Vector3d startLocOnSphere = this->projectToSphere(startLocation);
    Core::Vector3d endLocOnSphere = this->projectToSphere(endLocation);

    double angle = std::acos(startLocOnSphere.dot(endLocOnSphere) /
                             (startLocOnSphere.norm() * endLocOnSphere.norm()));
    angle = angle * psa::kDegreeMultiplier;
    Core::Vector3d axis = startLocOnSphere.cross(endLocOnSphere);
    axis.normalize();

    Core::Matrix3x3 eyeRotMatrix = m_viewpoint->eyeRotationMatrix();
    eyeRotMatrix.rotate(Core::Quaternion{axis, -angle});

    m_viewpoint->setEyeRotationMatrix(eyeRotMatrix);
    this->updateViewMatrix();
}

void ViewpointCamera::translate(const Core::Point3d& p)
{
    m_viewpoint->setTranslation(p);

    m_translationMatrix.makeIdentity();
    m_translationMatrix.translate(m_viewpoint->translation());

    this->updateViewMatrix();
}

void ViewpointCamera::updateViewMatrix()
{
    Core::Vector3d eye = m_viewpoint->eyeRotationMatrix() *
                         Core::Vector3d{0.0, 0.0, m_viewpointTranslation};
    eye.scale(m_viewpoint->zoomLevel());

    Core::Vector3d w = eye;
    w.normalize();

    Core::Vector3d upVector = m_viewpoint->eyeRotationMatrix() * Core::Vector3d{0.0, 1.0, 0.0};

    Core::Vector3d u = upVector.cross(w);
    u.normalize();

    Core::Vector3d v = w.cross(u);

    Core::Vector3d negativeEye{-eye.x(), -eye.y(), -eye.z()};

    m_viewMatrix = m_translationMatrix * Core::Matrix4x4{u.x(), v.x(), w.x(), 0.0,
        u.y(), v.y(), w.y(), 0.0,
        u.z(), v.z(), w.z(), 0.0,
        negativeEye.dot(u), negativeEye.dot(v), negativeEye.dot(w), 1.0};

    viewpointCameraChanged.emit_signal(); // emit signal
}

Core::Vector3d ViewpointCamera::projectToSphere(const Location& location)
{
    Core::Vector3d locOnSphere{location.x, location.y, 0.0};

    double norm = locOnSphere.norm();

    //if (norm < kSqrt2By2)
    //    locOnSphere.setZ(std::sqrt(1.0 - norm * norm));
    //else
    //    locOnSphere.setZ(0.5 / norm);

    if (norm > 1.0)
        locOnSphere.normalize();
    else
        locOnSphere.setZ(std::sqrt(1.0 - norm * norm));

    Core::Vector3d eye = Core::Vector3d{0.0, 0.0, m_viewpointTranslation}; // - center
    Core::Vector3d up = Core::Vector3d{0.0, 1.0, 0.0};

    Core::Vector3d side = up.cross(eye);

    up.setNorm(locOnSphere.y());
    side.setNorm(locOnSphere.x());
    eye.setNorm(locOnSphere.z());

    return up + side + eye;
}

} // namespace Graphics
