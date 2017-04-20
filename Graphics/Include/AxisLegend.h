/*
 * Graphics: Graphical objects for Thanuva Model/Geometry obejcts
 *
 * Copyright 2016, Saravanan Poosanthiram
 * All rights reserved.
 */

#ifndef GRAPHICS_AXISLEGEND_H
#define GRAPHICS_AXISLEGEND_H

#include <memory>
#include <vector>

#include "Matrix4x4.h"

namespace Geometry { class Geometry; }
namespace Model { class GeometryModel; }

namespace Graphics {

class GraphicsEnvironment;
class GraphicsObject;

class AxisLegend
{
public:
    AxisLegend(const GraphicsEnvironment& graphicsEnvironment);

    const Core::Matrix4x4& projectionMatrix() const { return m_projectionMatrix; }

    void adjustProjection();
    void render() const;

private:
    const GraphicsEnvironment& m_graphicsEnvironment;

    Core::Matrix4x4 m_projectionMatrix;

    std::vector<std::unique_ptr<Model::GeometryModel>> m_axisLegendModelObjects;
    std::vector<std::unique_ptr<Geometry::Geometry>> m_axisLegendGeometryObjects;
    std::vector<std::unique_ptr<Graphics::GraphicsObject>> m_axisLegendGraphicsObjects;
};

} // namespace Graphics

#endif // GRAPHICS_AXISLEGEND_H
