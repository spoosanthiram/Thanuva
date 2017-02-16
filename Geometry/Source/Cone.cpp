/*
 * Geometry: Geometry objects for Thanuva
 *
 * Copyright 2016, Saravanan Poosanthiram
 * All rights reserved.
 */

#include "Cone.h"

#include "GeometryCommon.h"

namespace Geometry {

void Cone::initialize(const Core::Point3d& apex, const Core::Point3d& center,
                      double radius, unsigned int numFacets)
{
    this->setExtent(Extent{std::numeric_limits<double>::infinity(), -std::numeric_limits<double>::infinity(),
                    std::numeric_limits<double>::infinity(), -std::numeric_limits<double>::infinity(),
                    std::numeric_limits<double>::infinity(), -std::numeric_limits<double>::infinity()},
                    Core::EmitSignal::DontEmit);

    // form a coordinate system with apex and center points
    Core::Vector3d w = apex - center;
    w.normalize();
    Core::Vector3d u = w.orthonormal();
    Core::Vector3d v = w.cross(u);

    // initial numFacets vectors
    std::vector<Core::Vector3d> initVectors = genInitialVectors(numFacets, u, v);

    this->generateBaseTriangles(center, radius, numFacets, initVectors, -w);
    this->generateSideTriangles(apex, numFacets, initVectors);
}

void Cone::generateBaseTriangles(const Core::Point3d& center, double radius, unsigned int numFacets,
                                 const std::vector<Core::Vector3d>& initVectors, const Core::Vector3d& normal)
{
    this->insertVertex(center);
    this->insertNormal(normal);

    for (size_t i = 0; i < initVectors.size(); ++i) {
        Core::Point3d p = center + radius * initVectors[i];
        this->insertVertex(p);
        this->insertNormal(normal);
    }

    for (unsigned int index = 0; index < numFacets; ++index)
        this->insertIndices(0, ((index + 1) % numFacets) + 1, index + 1);
}

void Cone::generateSideTriangles(const Core::Point3d& apex, unsigned int numFacets,
                                 const std::vector<Core::Vector3d>& initVectors)
{
    int aIndex = this->vertices().size() / 3;
    Core::Point3d a{&this->vertices()[3]};
    this->insertVertex(a);
    this->insertNormal(initVectors[0]);

    Core::Point3d b;
    int bIndex, apexIndex;
    for (unsigned int i = 1; i < numFacets; ++i) {
        bIndex = this->vertices().size() / 3;
        b.assign(&this->vertices()[(i + 1) * 3]);
        this->insertVertex(b);
        this->insertNormal(initVectors[i]);

        apexIndex = this->vertices().size() / 3;
        Core::Vector3d n = this->computeNormal(a, b, apex);
        this->insertVertex(apex);
        this->insertNormal(n);

        this->insertIndices(aIndex, bIndex, apexIndex);

        a = b;
        aIndex = bIndex;
    }

    bIndex = numFacets + 1;
    b.assign(&this->vertices()[(numFacets + 1) * 3]);

    apexIndex = this->vertices().size() / 3;
    Core::Vector3d n = this->computeNormal(a, b, apex);
    this->insertVertex(apex);
    this->insertNormal(n);

    this->insertIndices(aIndex, bIndex, apexIndex);
}

} // namespace Geometry
