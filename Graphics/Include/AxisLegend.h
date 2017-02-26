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

namespace Geometry { class GeometryObject; }
namespace Model { class ModelObject; }

namespace Graphics {

class GraphicsEnvironment;
class GraphicsObject;

class AxisLegend
{
public:
    AxisLegend(const GraphicsEnvironment& graphicsEnvironment);

    void render() const;

private:
    const GraphicsEnvironment& m_graphicsEnvironment;

    std::vector<std::unique_ptr<Model::ModelObject>> m_axisLegendModelObjects;
    std::vector<std::unique_ptr<Geometry::GeometryObject>> m_axisLegendGeometryObjects;
    std::vector<std::unique_ptr<Graphics::GraphicsObject>> m_axisLegendGraphicsObjects;
};

} // namespace Graphics

#endif // GRAPHICS_AXISLEGEND_H
