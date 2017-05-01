/*
 * Model: Model objects for Thanuva
 *
 * Copyright 2016, Saravanan Poosanthiram
 * All rights reserved.
 */

#ifndef Model_Viewpoint_h
#define Model_Viewpoint_h

#include "Matrix3x3.h"
#include "Point3d.h"
#include "ThanuvaModel.h"

namespace Model {

class Viewpoint : public ThanuvaModel
{
public:
    Viewpoint(const Scene* scene);

    double zoomLevel() const { return m_zoomLevel; }
    const Core::Matrix3x3& eyeRotationMatrix() const { return m_eyeRotationMatrix; }
    const Core::Point3d& translation() const { return m_translation; }

    void setZoomLevel(double zoomLevel);
    void setEyeRotationMatrix(const Core::Matrix3x3& eyeRotationMatrix);
    void setTranslation(const Core::Point3d& translation);

public:
    Nano::Signal<void()> zoomLevelChanged{};
    Nano::Signal<void()> eyeRotationMatrixChanged{};
    Nano::Signal<void()> translationChanged{};

protected:
    void loadModel(const boost::property_tree::ptree& modelPropTree);
    void saveModel(boost::property_tree::ptree& modelPropTree);

private:
    double m_zoomLevel{1.0};
    Core::Matrix3x3 m_eyeRotationMatrix{Core::Matrix3x3::identity()};
    Core::Point3d m_translation{};
};

} // namespace Model

#endif // Model_Viewpoint_h
