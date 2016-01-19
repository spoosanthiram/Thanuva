/**
* Author: Saravanan Poosanthiram
* $LastChangedBy: ps $
* $LastChangedDate: 2015-10-23 21:50:42 -0400 (Fri, 23 Oct 2015) $
*/

#include "ViewpointCameraModel.h"

#include <boost/property_tree/ptree.hpp>

#include "Quaternion.h"
#include "Vector3d.h"

namespace {

const char* kEyeRotationMatrixTag = "eyeRotationMatrix";
const char* kZoomLevelTag = "zoomLevel";

} // anonymous

namespace Model {

const double ViewpointCameraModel::kSqrt2By2 = 0.70710678118654752440084436210485;
const double ViewpointCameraModel::kZoomIncrement = 0.05;

ViewpointCameraModel::ViewpointCameraModel()
    : m_viewpointTranslation{3.0}
    , m_eyeRotationMatrix{Core::Matrix3x3::identity()}
    , m_zoomLevel{1.0}
{
}

void ViewpointCameraModel::setViewportTranslation(double viewportTranslation)
{
    m_viewpointTranslation = viewportTranslation;
    rotationChanged.emit_signal(); // emit signal
}

void ViewpointCameraModel::load(const boost::property_tree::ptree& cameraPropTree)
{
    m_eyeRotationMatrix.set(cameraPropTree.get<std::string>(kEyeRotationMatrixTag));
    m_zoomLevel = cameraPropTree.get<double>(kZoomLevelTag);
}

void ViewpointCameraModel::save(boost::property_tree::ptree& cameraPropTree)
{
    cameraPropTree.put(kEyeRotationMatrixTag, m_eyeRotationMatrix.str());
    cameraPropTree.put(kZoomLevelTag, m_zoomLevel);
}

void ViewpointCameraModel::rotate(const Location& startLocation, const Location& endLocation)
{
    Core::Vector3d startLocOnSphere = this->projectToSphere(startLocation);
    Core::Vector3d endLocOnSphere = this->projectToSphere(endLocation);

    double angle = std::acos(startLocOnSphere.dot(endLocOnSphere) / (startLocOnSphere.norm() * endLocOnSphere.norm()));
    angle = angle * psa::kDegreeMultiplier;
    Core::Vector3d axis = startLocOnSphere.cross(endLocOnSphere);
    axis.normalize();

    m_eyeRotationMatrix.rotate(Core::Quaternion{axis, -angle});

    rotationChanged.emit_signal(); // emit signal
}

void ViewpointCameraModel::zoom(int steps)
{
    m_zoomLevel += steps * kZoomIncrement;
    zoomChanged.emit_signal(); // emit signal
}

Core::Vector3d ViewpointCameraModel::projectToSphere(const Location& location)
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

    Core::Vector3d eye = /*m_eyeRotationMatrix **/ Core::Vector3d{0.0, 0.0, m_viewpointTranslation}; // - center
    Core::Vector3d up = /*m_eyeRotationMatrix **/ Core::Vector3d{0.0, 1.0, 0.0};

    Core::Vector3d side = up.cross(eye);

    up.setNorm(locOnSphere.y());
    side.setNorm(locOnSphere.x());
    eye.setNorm(locOnSphere.z());

    return up + side + eye;
}

} // namespace Model
