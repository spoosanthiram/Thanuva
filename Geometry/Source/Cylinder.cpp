/*
 * Geometry: Geometry objects for Thanuva
 *
 * Copyright 2016, Saravanan Poosanthiram
 * All rights reserved.
 */

#include "Cylinder.h"

#include "CylinderModel.h"
#include "GeometryCommon.h"

namespace Geometry {

Cylinder::Cylinder(const SceneGeometry* sceneGeometry, Model::CylinderModel* cylinderModel)
    : Geometry{sceneGeometry, cylinderModel}
{
    this->initialize();

    cylinderModel->endpoint1Changed.connect<Cylinder, &Cylinder::initialize>(this);
}

void Cylinder::initialize()
{
    this->clear();

    this->setBoundingBox(Extent{std::numeric_limits<double>::infinity(), -std::numeric_limits<double>::infinity(),
                         std::numeric_limits<double>::infinity(), -std::numeric_limits<double>::infinity(),
                         std::numeric_limits<double>::infinity(), -std::numeric_limits<double>::infinity()});

    auto cylinderModel = dynamic_cast<Model::CylinderModel*>(this->thanuvaModel());

    // form a coordinate system with center line
    Core::Vector3d w = cylinderModel->endpoint2() - cylinderModel->endpoint1();
    w.normalize();
    Core::Vector3d u = w.orthonormal();
    Core::Vector3d v = w.cross(u);

    // initial numFacets vectors
    std::vector<Core::Vector3d> initVectors = genInitialVectors(cylinderModel->numFacets(), u, v);

    this->generateFace1Triangles(initVectors, -w);
    this->generateFace2Triangles(initVectors, w);
    this->generateSideTriangles(initVectors);

    this->initializeBoundingBox();
    this->updateExtent();

    // emit signals
    geometryChanged.emit_signal();
    extentChanged.emit_signal();
}

void Cylinder::generateFace1Triangles(const std::vector<Core::Vector3d>& initVectors,
                                      const Core::Vector3d& normal)
{
    auto cylinderModel = dynamic_cast<Model::CylinderModel*>(this->thanuvaModel());

    this->generateFaceVertices(cylinderModel->endpoint1(), cylinderModel->radius1(), initVectors, normal);

    unsigned int numFacets = cylinderModel->numFacets();
    for (unsigned int index = 0; index < numFacets; ++index)
        this->insertIndices(0, ((index + 1) % numFacets) + 1, index + 1);
}

void Cylinder::generateFace2Triangles(const std::vector<Core::Vector3d>& initVectors,
                                      const Core::Vector3d& normal)
{
    unsigned int indexOffset = static_cast<unsigned int>(this->vertices().size() / 3);

    auto cylinderModel = dynamic_cast<Model::CylinderModel*>(this->thanuvaModel());

    this->generateFaceVertices(cylinderModel->endpoint2(), cylinderModel->radius2(), initVectors, normal);

    unsigned int numFacets = cylinderModel->numFacets();
    for (unsigned int i = 0; i < numFacets; ++i)
        this->insertIndices(indexOffset, indexOffset + i + 1, indexOffset + ((i + 1) % numFacets) + 1);
}

void Cylinder::generateFaceVertices(const Core::Point3d& endpoint, double radius,
                                    const std::vector<Core::Vector3d>& initVectors,
                                    const Core::Vector3d& normal)
{
    this->insertVertex(endpoint);
    this->insertNormal(normal);

    for (size_t i = 0; i < initVectors.size(); ++i) {
        Core::Point3d p = endpoint + radius * initVectors[i];
        this->insertVertex(p);
        this->insertNormal(normal);
    }
}

void Cylinder::generateSideTriangles(const std::vector<Core::Vector3d>& initVectors)
{
    unsigned int indexOffset = static_cast<unsigned int>(this->vertices().size() / 3);

    unsigned int numFacets = dynamic_cast<Model::CylinderModel*>(this->thanuvaModel())->numFacets();

    this->duplicateVertices(3, numFacets * 3);
    this->duplicateVertices((numFacets + 2) * 3, numFacets * 3);

    for (size_t i = 0; i < initVectors.size(); ++i)
        this->insertNormal(initVectors[i]);
    for (size_t i = 0; i < initVectors.size(); ++i)
        this->insertNormal(initVectors[i]);

    for (unsigned int i = 0; i < numFacets; ++i) {
        this->insertIndices(indexOffset + i,
                            indexOffset + (i + 1) % numFacets,
                            indexOffset + numFacets + i);
        this->insertIndices(indexOffset + (i + 1) % numFacets,
                            indexOffset + numFacets + ((i + 1) % numFacets),
                            indexOffset + numFacets + i);
    }
}

} // namespace Geometry
