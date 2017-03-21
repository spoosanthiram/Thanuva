/*
 * Geometry: Geometry objects for Thanuva
 *
 * Copyright 2016, Saravanan Poosanthiram
 * All rights reserved.
 */

#include "Mesh.h"

#include <fstream>
#include <thread>

#include <glog/logging.h>

#include "MeshReader.h"
#include "MeshModel.h"
#include "Utils.h"

namespace Geometry {

Mesh::Mesh(const GeometryContainer* geometryContainer, Model::MeshModel* meshModel)
    : GeometryObject{geometryContainer, meshModel}
{
    this->initialize();

    meshModel->filePathChanged.connect<Mesh, &Mesh::initialize>(this);
}

void Mesh::initialize()
{
    fs::path filePath = static_cast<Model::MeshModel*>(this->modelObject())->filePath();
    auto reader = MeshReader::getInstance(filePath);
    if (!reader)
        return;

    this->clear();
    this->setBoundingBox(Extent{std::numeric_limits<double>::infinity(), -std::numeric_limits<double>::infinity(),
                         std::numeric_limits<double>::infinity(), -std::numeric_limits<double>::infinity(),
                         std::numeric_limits<double>::infinity(), -std::numeric_limits<double>::infinity()});

    reader->read(*this);

    this->initializeBoundingBox();
    this->updateExtent();

    // emit signals
    geometryObjectChanged.emit_signal();
    extentChanged.emit_signal();
}

} // namespace Geometry
