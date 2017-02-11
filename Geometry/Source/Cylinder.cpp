/*
 * Geometry: Geometry objects for Thanuva
 *
 * Copyright 2016, Saravanan Poosanthiram
 * All rights reserved.
 */

#include "Cylinder.h"

#include "CylinderModel.h"

namespace Geometry {

Cylinder::Cylinder(const GeometryContainer& geometryContainer, Model::CylinderModel* cylinderModel)
    : GeometryObject(geometryContainer, cylinderModel)
{
    this->initialize();

    cylinderModel->endpoint1Changed.connect<Cylinder, &Cylinder::initialize>(this);
}

void Cylinder::initialize()
{
    this->clear();
    this->setExtent(Extent{std::numeric_limits<double>::infinity(), -std::numeric_limits<double>::infinity(),
                    std::numeric_limits<double>::infinity(), -std::numeric_limits<double>::infinity(),
                    std::numeric_limits<double>::infinity(), -std::numeric_limits<double>::infinity()},
                    Core::EmitSignal::DontEmit);

    auto cylinderModel = dynamic_cast<Model::CylinderModel*>(this->modelObject());

    Core::Vector3d w = cylinderModel->endpoint2() - cylinderModel->endpoint1();
    w.normalize();
    Core::Vector3d u = w.orthonormal();
    Core::Vector3d v = w.cross(u);

    std::vector<Core::Vector3d> initVectors = genInitialVectors(cylinderModel->numFacets(), u, v);

    this->generateFace1Triangles(initVectors, -w);
    this->generateFace2Triangles(initVectors, w);
    this->generateSideTriangles(initVectors);

    // emit signals
    geometryObjectChanged.emit_signal();
    extentChanged.emit_signal();
}

std::vector<Core::Vector3d> Cylinder::genInitialVectors(unsigned int numFacets,
                                                        const Core::Vector3d& u, const Core::Vector3d& v)
{
    std::vector<Core::Vector3d> vectors{};
    vectors.reserve(numFacets * 3);

    double deltaAngle = (2.0 * psa::kPi) / numFacets;
    double angle = 0.0;
    for (unsigned int i = 0; i < numFacets; ++i) {
        Core::Vector3d x = (std::cos(angle) * u) + (std::sin(angle) * v);
        vectors.push_back(x);

        angle += deltaAngle;
    }

    return vectors;
}

void Cylinder::generateFace1Triangles(const std::vector<Core::Vector3d>& initVectors,
                                      const Core::Vector3d& normal)
{
    auto cylinderModel = dynamic_cast<Model::CylinderModel*>(this->modelObject());

    this->generateFaceVertices(cylinderModel->endpoint1(), cylinderModel->radius1(), initVectors, normal);

    unsigned int numFacets = cylinderModel->numFacets();
    for (unsigned int index = 0; index < numFacets; ++index)
        this->insertIndices(0, ((index + 1) % numFacets) + 1, index + 1);
}

void Cylinder::generateFace2Triangles(const std::vector<Core::Vector3d>& initVectors,
                                      const Core::Vector3d& normal)
{
    unsigned int indexOffset = this->vertices().size() / 3;

    auto cylinderModel = dynamic_cast<Model::CylinderModel*>(this->modelObject());

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
    unsigned int indexOffset = this->vertices().size() / 3;

    auto cylinderModel = dynamic_cast<Model::CylinderModel*>(this->modelObject());
    unsigned int numFacets = cylinderModel->numFacets();

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
