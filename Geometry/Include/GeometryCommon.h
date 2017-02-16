/*
 * Geometry: Geometry objects for Thanuva
 *
 * Copyright 2016, Saravanan Poosanthiram
 * All rights reserved.
 */

#ifndef GEOMETRY_GEOMETRYCOMMON_H
#define GEOMETRY_GEOMETRYCOMMON_H

#include <vector>

#include "Point3d.h"

namespace Geometry {

/**
 * generates numVectors around {u, v} plane
 */
std::vector<Core::Vector3d> genInitialVectors(unsigned int numVectors,
                                              const Core::Vector3d& u, const Core::Vector3d& v);
} // namespace Geometry

#endif // GEOMETRY_GEOMETRYCOMMON_H
