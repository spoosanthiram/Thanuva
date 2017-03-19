/*
 * Geometry: Geometry objects for Thanuva
 *
 * Copyright 2016, Saravanan Poosanthiram
 * All rights reserved.
 */

#include "Sphere.h"

#include "SphereModel.h"

namespace Geometry {

Sphere::Sphere(const GeometryContainer* geometryContainer, Model::SphereModel* sphereModel)
    : GeometryObject{geometryContainer, sphereModel}
{
    this->initialize();

    sphereModel->centerChanged.connect<Sphere, &Sphere::initialize>(this);
}

void Sphere::initialize()
{
    this->clear();

    this->setExtent(Extent{std::numeric_limits<double>::infinity(), -std::numeric_limits<double>::infinity(),
                    std::numeric_limits<double>::infinity(), -std::numeric_limits<double>::infinity(),
                    std::numeric_limits<double>::infinity(), -std::numeric_limits<double>::infinity()},
                    Core::EmitSignal::DontEmit);

    this->initializeOctahedron();
    //auto sphereModel = dynamic_cast<Model::SphereModel*>(this->modelObject());

    this->initializeBoundingBox();

    // emit signals
    geometryObjectChanged.emit_signal();
    extentChanged.emit_signal();
}

void Sphere::initializeOctahedron()
{
    // bottom vertex
    Core::Point3d v0{0.0, 0.0, -1.0};
    this->insertVertex(v0.data());
    this->insertNormal(v0.data());

    // middle vertices
    Core::Point3d v1{1.0, 0.0, 0.0};
    this->insertVertex(v1.data());
    this->insertNormal(v1.data());
    Core::Point3d v2{0.0, 1.0, 0.0};
    this->insertVertex(v2.data());
    this->insertNormal(v2.data());
    Core::Point3d v3{-1.0, 0.0, 0.0};
    this->insertVertex(v3.data());
    this->insertNormal(v3.data());
    Core::Point3d v4{0.0, -1.0, 0.0};
    this->insertVertex(v4.data());
    this->insertNormal(v4.data());

    // top vertex
    Core::Point3d v5{0.0, 0.0, 1.0};
    this->insertVertex(v5.data());
    this->insertNormal(v5.data());

    // Facets
    this->insertIndices(0, 2, 1);
    this->insertIndices(0, 3, 2);
    this->insertIndices(0, 4, 3);
    this->insertIndices(0, 4, 1);

    this->insertIndices(1, 2, 5);
    this->insertIndices(2, 3, 5);
    this->insertIndices(3, 4, 5);
    this->insertIndices(4, 1, 5);
}

} // namespace Geometry
