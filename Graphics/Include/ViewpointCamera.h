/*
 * Graphics: Graphical objects for Thanuva Model/Geometry obejcts
 *
 * Copyright 2016, Saravanan Poosanthiram
 * All rights reserved.
 */

#ifndef Graphics_ViewpointCamera_h
#define Graphics_ViewpointCamera_h

#include <nano_signal_slot.hpp>

#include "Matrix3x3.h"
#include "Matrix4x4.h"
#include "Viewpoint.h"

namespace Graphics {

class AbstractGraphicsScene;

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
    ViewpointCamera(const AbstractGraphicsScene& graphicsScene);

    const Core::Matrix4x4& viewMatrix() const { return m_viewMatrix; }

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

    void zoom(int steps);
    void rotate(const Location& startLocation, const Location& endLocation);
    void translate(const Core::Point3d& p);

public: // signals
    Nano::Signal<void()> viewpointCameraChanged{};

protected: // slots
    void updateViewMatrix();

private:
    Core::Vector3d projectToSphere(const Location& location);

    const AbstractGraphicsScene& m_graphicsScene;

    double m_viewpointTranslation{3.0};
    Model::Viewpoint* m_viewpoint;

    Core::Matrix4x4 m_translationMatrix;
    Core::Matrix4x4 m_viewMatrix;
};

} // namespace Graphics

#endif // Graphics_ViewpointCamera_h
