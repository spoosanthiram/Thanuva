/*
 * Geometry: Geometry objects for Thanuva
 *
 * Copyright 2016, Saravanan Poosanthiram
 * All rights reserved.
 */

#include "Cone.h"

#include "ConeModel.h"
#include "GeometryCommon.h"

namespace Geometry {

Cone::Cone(const GeometryContainer* geometryContainer, Model::ConeModel* coneModel)
    : GeometryObject{geometryContainer, coneModel}
{
    this->initialize();

    coneModel->apexChanged.connect<Cone, &Cone::initialize>(this);
}

void Cone::initialize()
{
    this->clear();

    this->setExtent(Extent{std::numeric_limits<double>::infinity(), -std::numeric_limits<double>::infinity(),
                    std::numeric_limits<double>::infinity(), -std::numeric_limits<double>::infinity(),
                    std::numeric_limits<double>::infinity(), -std::numeric_limits<double>::infinity()},
                    Core::EmitSignal::DontEmit);

    auto coneModel = dynamic_cast<Model::ConeModel*>(this->modelObject());

    // form a coordinate system with apex and center points
    Core::Vector3d w = coneModel->apex() - coneModel->center();
    w.normalize();
    Core::Vector3d u = w.orthonormal();
    Core::Vector3d v = w.cross(u);

    // initial numFacets vectors
    std::vector<Core::Vector3d> initVectors = genInitialVectors(coneModel->numFacets(), u, v);

    this->generateBaseTriangles(initVectors, -w);
    this->generateSideTriangles(initVectors);

    this->initializeBoundingBox();

    // emit signals
    geometryObjectChanged.emit_signal();
    extentChanged.emit_signal();
}

void Cone::generateBaseTriangles(const std::vector<Core::Vector3d>& initVectors, const Core::Vector3d& normal)
{
    auto coneModel = dynamic_cast<Model::ConeModel*>(this->modelObject());

    const Core::Point3d& center = coneModel->center();
    this->insertVertex(center);
    this->insertNormal(normal);

    double radius = coneModel->radius();
    for (size_t i = 0; i < initVectors.size(); ++i) {
        Core::Point3d p = center + radius * initVectors[i];
        this->insertVertex(p);
        this->insertNormal(normal);
    }

    unsigned int numFacets = coneModel->numFacets();
    for (unsigned int index = 0; index < numFacets; ++index)
        this->insertIndices(0, ((index + 1) % numFacets) + 1, index + 1);
}

void Cone::generateSideTriangles(const std::vector<Core::Vector3d>& initVectors)
{
    int aIndex = static_cast<int>(this->vertices().size() / 3);
    Core::Point3d a{&this->vertices()[3]};
    this->insertVertex(a);
    this->insertNormal(initVectors[0]);

    auto coneModel = dynamic_cast<Model::ConeModel*>(this->modelObject());
    const Core::Point3d& apex = coneModel->apex();
    unsigned int numFacets = coneModel->numFacets();

    Core::Point3d b;
    int bIndex, apexIndex;
    for (unsigned int i = 1; i < numFacets; ++i) {
        bIndex = static_cast<int>(this->vertices().size() / 3);
        b.assign(&this->vertices()[(i + 1) * 3]);
        this->insertVertex(b);
        this->insertNormal(initVectors[i]);

        apexIndex = static_cast<int>(this->vertices().size() / 3);
        Core::Vector3d n = this->computeNormal(a, b, apex);
        this->insertVertex(apex);
        this->insertNormal(n);

        this->insertIndices(aIndex, bIndex, apexIndex);

        a = b;
        aIndex = bIndex;
    }

    bIndex = numFacets + 1;
    b.assign(&this->vertices()[(numFacets + 1) * 3]);

    apexIndex = static_cast<int>(this->vertices().size() / 3);
    Core::Vector3d n = this->computeNormal(a, b, apex);
    this->insertVertex(apex);
    this->insertNormal(n);

    this->insertIndices(aIndex, bIndex, apexIndex);
}

} // namespace Geometry
