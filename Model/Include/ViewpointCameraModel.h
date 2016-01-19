/**
* Author: Saravanan Poosanthiram
* $LastChangedBy: ps $
* $LastChangedDate: 2015-03-20 18:08:01 -0400 (Fri, 20 Mar 2015) $
*/

#ifndef MODEL_VIEWPOINTCAMERAMODEL_H
#define MODEL_VIEWPOINTCAMERAMODEL_H

#include "Matrix3x3.h"

#include <boost/property_tree/ptree_fwd.hpp>

#include "nano_signal_slot.hpp"

namespace Model {

class ViewpointCameraModel {
public:
    static const double kSqrt2By2;
    static const double kZoomIncrement;

    struct Location {
        Location() : x{0.0}, y{0.0} {}
        Location(double xval, double yval) : x{xval}, y{yval} {}
        double x;
        double y;
    };

public:
    ViewpointCameraModel();

    double viewpointTranslation() const { return m_viewpointTranslation; }
    const Core::Matrix3x3& eyeRotationMatrix() const { return m_eyeRotationMatrix; }
    double zoomLevel() const { return m_zoomLevel; }

    void setViewportTranslation(double viewportTranslation);

    void load(const boost::property_tree::ptree& cameraPropTree);
    void save(boost::property_tree::ptree& cameraPropTree);

    void rotate(const Location& startLocation, const Location& endLocation);
    void zoom(int steps);

public: // signals
    Nano::Signal<void()> rotationChanged;
    Nano::Signal<void()> zoomChanged;

protected:
    Core::Vector3d projectToSphere(const Location& location);

private:
    double m_viewpointTranslation;
    Core::Matrix3x3 m_eyeRotationMatrix;
    double m_zoomLevel;
};

} // namespace Model

#endif // MODEL_VIEWPOINTCAMERAMODEL_H
