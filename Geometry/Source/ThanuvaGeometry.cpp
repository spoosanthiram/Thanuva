/*
 * Geometry: Geometry objects for Thanuva
 *
 * Copyright 2016, Saravanan Poosanthiram
 * All rights reserved.
 */

#include "ThanuvaGeometry.h"

#include <glog/logging.h>

#include "ThanuvaModel.h"

namespace Geometry {

ThanuvaGeometry::ThanuvaGeometry(const GeometryScene* geometryScene, Model::ThanuvaModel* thanuvaModel)
    : m_geometryScene{geometryScene}
    , m_thanuvaModel{thanuvaModel}
{
    CHECK(thanuvaModel);
}

} // namespace Geometry
