/*
 * Geometry: Geometry objects for Thanuva
 *
 * Copyright 2016, Saravanan Poosanthiram
 * All rights reserved.
 */

#include "CoordinateSystem.h"

#include "CoordinateSystemModel.h"
#include "HPoint.h"
#include "HVector.h"

namespace Geometry {

CoordinateSystem::CoordinateSystem(const SceneGeometry* sceneGeometry,
                                   Model::CoordinateSystemModel* csysModel)
    : ThanuvaGeometry{sceneGeometry, csysModel}
{
    Core::Point3d origin = csysModel->origin();
    Core::Vector3d xAxis = csysModel->xAxis();
    xAxis.normalize();
    Core::Vector3d zAxis = xAxis.cross(csysModel->yAxis());
    zAxis.normalize();
    Core::Vector3d yAxis = zAxis.cross(xAxis);

    m_transformMatrix.setColumn(0, Core::HVector{xAxis, 0.0});
    m_transformMatrix.setColumn(1, Core::HVector{yAxis, 0.0});
    m_transformMatrix.setColumn(2, Core::HVector{zAxis, 0.0});
    m_transformMatrix.setColumn(3, Core::HPoint{origin, 1.0});

    //csysModel->originChanged.connect<>();
    //csysModel->xAxisChanged.connect<>();
    //csysModel->yAxisChanged.connect<>();
}

} // namespace Geometry
