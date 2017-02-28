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

class GraphicsEnvironment;

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
    ViewpointCamera(const GraphicsEnvironment& graphicsEnvironment);

    const Core::Matrix4x4& viewMatrix() const { return m_viewMatrix; }
    const Core::Matrix4x4& legendViewMatrix() const { return m_legendViewMatrix; }

    void setViewpointTranslation(double viewpointTranslation)
    {
        m_viewpointTranslation = viewpointTranslation;
        this->updateViewMatrix();
    }
    void setViewpoint(Model::Viewpoint* viewpoint)
    {
        m_viewpoint = viewpoint;
        this->updateViewMatrix();
    }
    void adjustLegendTranslation();

    void rotate(const Location& startLocation, const Location& endLocation);
    void zoom(int steps);

public: // signals
    Nano::Signal<void()> viewpointCameraChanged{};

protected: // slots
    void updateViewMatrix();

private:
    Core::Vector3d projectToSphere(const Location& location);
    Core::Matrix4x4 buildViewMatrix(const Core::Vector3d& eye);

    const GraphicsEnvironment& m_graphicsEnvironment;

    double m_viewpointTranslation{3.0};
    Model::Viewpoint* m_viewpoint;

    Core::Matrix4x4 m_viewMatrix;

    Core::Matrix4x4 m_legendTranslationMatrix;
    Core::Matrix4x4 m_legendViewMatrix;
};

} // namespace Graphics

#endif // GRAPHICS_VIEWPOINTCAMERA_H
