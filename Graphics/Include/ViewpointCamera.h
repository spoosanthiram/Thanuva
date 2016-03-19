/*
 * Graphics: Graphical objects for Thanuva Model/Geometry obejcts
 *
 * Copyright 2016, Saravanan Poosanthiram
 * All rights reserved.
 */

#ifndef GRAPHICS_VIEWPOINTCAMERA_H
#define GRAPHICS_VIEWPOINTCAMERA_H

#include <nano_signal_slot.hpp>

#include "Matrix3x3.h"
#include "Matrix4x4.h"
#include "Viewpoint.h"

namespace Graphics {

class ViewpointCamera
{
public:
    static const double kZoomIncrement;

    struct Location
    {
        Location() : x{0.0}, y{0.0} {}
        Location(double xval, double yval) : x{xval}, y{yval} {}
        double x;
        double y;
    };

public:
    ViewpointCamera() : m_viewMatrix{Core::Matrix4x4::identity()} {}

    const Core::Matrix4x4& viewMatrix() const { return m_viewMatrix; }

    void setViewpointTranslation(double viewpointTranslation)
    {
        m_viewpointTranslation = viewpointTranslation;
    }
    void setViewpoint(const Model::Viewpoint& viewpoint) { m_viewpoint = viewpoint; }

    void rotate(const Location& startLocation, const Location& endLocation);
    void zoom(int steps);

public: // signals
    Nano::Signal<void()> viewpointCameraChanged{};

protected: // slots
    void updateViewMatrix();

private:
    Core::Vector3d projectToSphere(const Location& location);

    double m_viewpointTranslation{3.0};
    Model::Viewpoint m_viewpoint{};

    Core::Matrix4x4 m_viewMatrix;
};

} // namespace Graphics

#endif // GRAPHICS_VIEWPOINTCAMERA_H
