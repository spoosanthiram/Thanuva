/*
 * Graphics: Graphical objects for Thanuva Model/Geometry obejcts
 *
 * Copyright 2016, Saravanan Poosanthiram
 * All rights reserved.
 */

#include "AxisLegend.h"

#include "Cone.h"
#include "ConeModel.h"
#include "Cylinder.h"
#include "CylinderModel.h"
#include "GraphicsEnvironment.h"
#include "GraphicsObject.h"

namespace Graphics {

AxisLegend::AxisLegend(const GraphicsEnvironment& graphicsEnvironment)
    : m_graphicsEnvironment{graphicsEnvironment}
    , m_projectionMatrix{Core::Matrix4x4::identity()}
{
    // X Axis
    m_axisLegendModelObjects.push_back(std::make_unique<Model::CylinderModel>(
        nullptr, Core::Point3d{0.0, 0.0, 0.0}, 0.025, Core::Point3d{0.25, 0.0, 0.0}, 0.025, 32));
    m_axisLegendModelObjects.push_back(std::make_unique<Model::ConeModel>(
        nullptr, Core::Point3d{0.35, 0.0, 0.0}, Core::Point3d{0.25, 0.0, 0.0}, 0.05, 32));
    m_axisLegendModelObjects[0]->setMaterial({Core::Color{255, 0, 0}, Core::Color{255, 255, 255}, 128.0f});
    m_axisLegendModelObjects[1]->setMaterial({Core::Color{255, 0, 0}, Core::Color{255, 255, 255}, 128.0f});

    // Y Axis
    m_axisLegendModelObjects.push_back(std::make_unique<Model::CylinderModel>(
        nullptr, Core::Point3d{0.0, 0.0, 0.0}, 0.025, Core::Point3d{0.0, 0.25, 0.0}, 0.025, 32));
    m_axisLegendModelObjects.push_back(std::make_unique<Model::ConeModel>(
        nullptr, Core::Point3d{0.0, 0.35, 0.0}, Core::Point3d{0.0, 0.25, 0.0}, 0.05, 32));
    m_axisLegendModelObjects[2]->setMaterial({Core::Color{0, 255, 0}, Core::Color{255, 255, 255}, 128.0f});
    m_axisLegendModelObjects[3]->setMaterial({Core::Color{0, 255, 0}, Core::Color{255, 255, 255}, 128.0f});

    // Z Axis
    m_axisLegendModelObjects.push_back(std::make_unique<Model::CylinderModel>(
        nullptr, Core::Point3d{0.0, 0.0, 0.0}, 0.025, Core::Point3d{0.0, 0.0, 0.25}, 0.025, 32));
    m_axisLegendModelObjects.push_back(std::make_unique<Model::ConeModel>(
        nullptr, Core::Point3d{0.0, 0.0, 0.35}, Core::Point3d{0.0, 0.0, 0.25}, 0.05, 32));
    m_axisLegendModelObjects[4]->setMaterial({Core::Color{0, 0, 255}, Core::Color{255, 255, 255}, 128.0f});
    m_axisLegendModelObjects[5]->setMaterial({Core::Color{0, 0, 255}, Core::Color{255, 255, 255}, 128.0f});

    for (int i = 0; i < 6; i += 2) {
        m_axisLegendGeometryObjects.push_back(std::make_unique<Geometry::Cylinder>(
            nullptr, dynamic_cast<Model::CylinderModel*>(m_axisLegendModelObjects[i].get())));
        m_axisLegendGeometryObjects.push_back(std::make_unique<Geometry::Cone>(
            nullptr, dynamic_cast<Model::ConeModel*>(m_axisLegendModelObjects[i + 1].get())));
    }

    for (int i = 0; i < 6; i += 2) {
        m_axisLegendGraphicsObjects.push_back(std::make_unique<GraphicsObject>(
            m_graphicsEnvironment, m_axisLegendGeometryObjects[i].get()));
        m_axisLegendGraphicsObjects.push_back(std::make_unique<GraphicsObject>(
            m_graphicsEnvironment, m_axisLegendGeometryObjects[i + 1].get()));
    }
}

void AxisLegend::adjustProjection()
{
    //m_projectionMatrix = Core::Matrix4x4::perspective(60.0, m_graphicsEnvironment.windowAspect(),
    //                                                  0.1, GraphicsEnvironment::kLegendFarProjection);
    double halfLen = GraphicsEnvironment::kLegendFarProjection / 2.0;
    m_projectionMatrix = Core::Matrix4x4::ortho(
        -halfLen * m_graphicsEnvironment.windowAspect(), halfLen * m_graphicsEnvironment.windowAspect(),
        -halfLen, halfLen, 0.0, GraphicsEnvironment::kLegendFarProjection);
}

void AxisLegend::render() const
{
    for (auto& graphicsObject : m_axisLegendGraphicsObjects)
        graphicsObject->render(true);
}

} // namespace Graphics
