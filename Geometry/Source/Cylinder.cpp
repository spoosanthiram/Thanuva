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

Cylinder::Cylinder(const GeometryContainer* geometryContainer, Model::CylinderModel* cylinderModel)
    : GeometryObject{geometryContainer, cylinderModel}
{
    this->initialize();

    cylinderModel->endpoint1Changed.connect<Cylinder, &Cylinder::initialize>(this);
}

void Cylinder::initialize()
{
    this->clear();

    auto cylinderModel = dynamic_cast<Model::CylinderModel*>(this->modelObject());
    this->initialize(cylinderModel->endpoint1(), cylinderModel->radius1(),
                     cylinderModel->endpoint2(), cylinderModel->radius2(),
                     cylinderModel->numFacets());

    this->initializeBoundingBox();

    // emit signals
    geometryObjectChanged.emit_signal();
    extentChanged.emit_signal();
}

void Cylinder::initialize(const Core::Point3d& endpoint1, double radius1,
                          const Core::Point3d& endpoint2, double radius2,
                          unsigned int numFacets)
{
    this->setExtent(Extent{std::numeric_limits<double>::infinity(), -std::numeric_limits<double>::infinity(),
                    std::numeric_limits<double>::infinity(), -std::numeric_limits<double>::infinity(),
                    std::numeric_limits<double>::infinity(), -std::numeric_limits<double>::infinity()},
                    Core::EmitSignal::DontEmit);

    // form a coordinate system with center line
    Core::Vector3d w = endpoint2 - endpoint1;
    w.normalize();
    Core::Vector3d u = w.orthonormal();
    Core::Vector3d v = w.cross(u);

    // initial numFacets vectors
    std::vector<Core::Vector3d> initVectors = genInitialVectors(numFacets, u, v);

    this->generateFace1Triangles(endpoint1, radius1, numFacets, initVectors, -w);
    this->generateFace2Triangles(endpoint2, radius2, numFacets, initVectors, w);
    this->generateSideTriangles(numFacets, initVectors);
}

void Cylinder::generateFace1Triangles(const Core::Point3d& endpoint1, double radius1, unsigned int numFacets,
                                      const std::vector<Core::Vector3d>& initVectors,
                                      const Core::Vector3d& normal)
{
    this->generateFaceVertices(endpoint1, radius1, initVectors, normal);

    for (unsigned int index = 0; index < numFacets; ++index)
        this->insertIndices(0, ((index + 1) % numFacets) + 1, index + 1);
}

void Cylinder::generateFace2Triangles(const Core::Point3d& endpoint2, double radius2, unsigned int numFacets,
                                      const std::vector<Core::Vector3d>& initVectors,
                                      const Core::Vector3d& normal)
{
    unsigned int indexOffset = this->vertices().size() / 3;

    this->generateFaceVertices(endpoint2, radius2, initVectors, normal);

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

void Cylinder::generateSideTriangles(unsigned int numFacets, const std::vector<Core::Vector3d>& initVectors)
{
    unsigned int indexOffset = this->vertices().size() / 3;

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
