/*
 * Geometry: Geometry objects for Thanuva
 *
 * Copyright 2016, Saravanan Poosanthiram
 * All rights reserved.
 */

#include "GeometryCommon.h"

namespace Geometry {

std::vector<Core::Vector3d> genInitialVectors(unsigned int numFacets,
                                              const Core::Vector3d& u, const Core::Vector3d& v)
{
    std::vector<Core::Vector3d> vectors{};
    vectors.reserve(numFacets);

    double deltaAngle = (2.0 * psa::kPi) / numFacets;
    double angle = 0.0;
    for (unsigned int i = 0; i < numFacets; ++i) {
        Core::Vector3d x = (std::cos(angle) * u) + (std::sin(angle) * v);
        vectors.emplace_back(x);

        angle += deltaAngle;
    }

    return vectors;
}

} // namespace Geometry
