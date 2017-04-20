/*
 * Model: Model objects for Thanuva
 *
 * Copyright 2016, Saravanan Poosanthiram
 * All rights reserved.
 */

#ifndef Model_Viewpoint_h
#define Model_Viewpoint_h

#include "Matrix3x3.h"
#include "ThanuvaModel.h"

namespace Model {

class Viewpoint : public ThanuvaModel
{
public:
    Viewpoint(const Scene* scene);

    const Core::Matrix3x3& eyeRotationMatrix() const { return m_eyeRotationMatrix; }
    Core::Matrix3x3& eyeRotationMatrix() { return m_eyeRotationMatrix; }
    double zoomLevel() const { return m_zoomLevel; }

    //void setEyeRotationMatrix(const Core::Matrix3x3& eyeRotationMatrix);
    void setZoomLevel(double zoomLevel);

public:
    Nano::Signal<void()> zoomLevelChanged{};

protected:
    void loadModel(const boost::property_tree::ptree& modelPropTree);
    void saveModel(boost::property_tree::ptree& modelPropTree);

private:
    Core::Matrix3x3 m_eyeRotationMatrix{Core::Matrix3x3::identity()};
    double m_zoomLevel{1.0};
};

} // namespace Model

#endif // Model_Viewpoint_h
