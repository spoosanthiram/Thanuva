/*
 * Graphics: Graphical objects for Thanuva Model/Geometry obejcts
 *
 * Copyright 2016, Saravanan Poosanthiram
 * All rights reserved.
 */

#include "AxisLegend.h"

#include "Cylinder.h"
#include "CylinderModel.h"
#include "GraphicsObject.h"

namespace Graphics {

AxisLegend::AxisLegend(const GraphicsEnvironment& graphicsEnvironment)
    : m_graphicsEnvironment{graphicsEnvironment}
{
    m_axisLegendModelObjects.push_back(std::make_unique<Model::CylinderModel>(
            nullptr, Core::Point3d{0.0, 0.0, 0.0}, 0.1, Core::Point3d{1.0, 0.0, 0.0}, 0.2, 32));

    m_axisLegendGeometryObjects.push_back(std::make_unique<Geometry::Cylinder>(nullptr,
            dynamic_cast<Model::CylinderModel*>(m_axisLegendModelObjects.back().get())));

    m_axisLegendGraphicsObjects.push_back(std::make_unique<GraphicsObject>(
            m_graphicsEnvironment, m_axisLegendGeometryObjects.back().get()));
}

void AxisLegend::render() const
{
    m_axisLegendGraphicsObjects.back()->render();
}

} // namespace Graphics
