/*
 * Geometry: Geometry objects for Thanuva
 *
 * Copyright 2016, Saravanan Poosanthiram
 * All rights reserved.
 */

#include "Box.h"

#include <nano_signal_slot.hpp>

#include "BoxModel.h"

namespace Geometry {

Box::Box(const GeometryContainer& geometryContainer, Model::BoxModel* boxModel)
    : GeometryObject(geometryContainer, boxModel)
{
    this->initialize();

    boxModel->limiterChanged.connect<Box, &Box::initialize>(this);
}

void Box::initialize()
{
    this->clear();

    this->setExtent(Extent{std::numeric_limits<double>::infinity(), -std::numeric_limits<double>::infinity(),
                        std::numeric_limits<double>::infinity(), -std::numeric_limits<double>::infinity(),
                        std::numeric_limits<double>::infinity(), -std::numeric_limits<double>::infinity()},
                    Core::EmitSignal::DontEmit);

    this->reserve(24 * GeometryObject::kValuesPerVertex, 24 * GeometryObject::kValuesPerVertex, 36);

    auto boxModel = dynamic_cast<Model::BoxModel*>(this->modelObject());
    const Model::BoxModel::Limiter& limiter = boxModel->limiter();

    Core::Point3d a{limiter.xlow, limiter.ylow, limiter.zlow};
    Core::Point3d b{limiter.xhigh, limiter.ylow, limiter.zlow};
    Core::Point3d c{limiter.xhigh, limiter.yhigh, limiter.zlow};
    Core::Point3d d{limiter.xlow, limiter.yhigh, limiter.zlow};
    Core::Point3d e{limiter.xlow, limiter.ylow, limiter.zhigh};
    Core::Point3d f{limiter.xhigh, limiter.ylow, limiter.zhigh};
    Core::Point3d g{limiter.xhigh, limiter.yhigh, limiter.zhigh};
    Core::Point3d h{limiter.xlow, limiter.yhigh, limiter.zhigh};

    this->insertQuad(b, c, g, f); // X Max plane
    this->insertQuad(d, a, e, h); // X Min plane

    this->insertQuad(c, d, h, g); // Y Max plane
    this->insertQuad(a, b, f, e); // Y Min plane

    this->insertQuad(e, f, g, h); // Z Max plane
    this->insertQuad(d, c, b, a); // Z Min plane

    this->initializeBoundingBox();

    // emit signals
    geometryObjectChanged.emit_signal();
    extentChanged.emit_signal();
}

void Box::insertQuad(const Core::Point3d& a, const Core::Point3d& b,
                     const Core::Point3d& c, const Core::Point3d& d)
{
    int index = static_cast<int>(this->vertices().size() / kValuesPerVertex);

    this->insertVertex(a);
    this->insertVertex(b);
    this->insertVertex(c);
    this->insertVertex(d);

    Core::Vector3d n = this->computeNormal(a, b, c);

    this->insertNormal(n);
    this->insertNormal(n);
    this->insertNormal(n);
    this->insertNormal(n);

    // a, b, c triangle
    this->insertIndices(index, index + 1, index + 2);

    // a, c, d triangle
    this->insertIndices(index, index + 2, index + 3);
}

} // namespace Geometry
