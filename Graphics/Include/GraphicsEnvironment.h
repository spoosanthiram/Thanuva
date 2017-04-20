/*
 * Graphics: Graphical objects for Thanuva Model/Geometry obejcts
 *
 * Copyright 2016, Saravanan Poosanthiram
 * All rights reserved.
 */

#ifndef GRAPHICS_GRAPHICSENVIRONMENT_H
#define GRAPHICS_GRAPHICSENVIRONMENT_H

#include <memory>
#include <vector>

#include <QObject>

#include "AxisLegend.h"
#include "GraphicsObject.h"
#include "SceneGeometry.h"
#include "ShaderProgram.h"
#include "ViewpointCamera.h"

namespace Graphics {

class GraphicsEnvironment : public QObject
{
    Q_OBJECT

public:
    static const double kViewpointTranslationMultiplier;
    static const double kFarProjectionMultiplier;
    static const double kLegendFarProjection;

public:
    GraphicsEnvironment();

    const std::unique_ptr<Geometry::SceneGeometry>& sceneGeometry() const { return m_sceneGeometry; }
    const std::unique_ptr<ShaderProgram>& shaderProgram() const { return m_shaderProgram; }
    const ViewpointCamera& viewpointCamera() const { return m_viewpointCamera; }
    ViewpointCamera& viewpointCamera() { return m_viewpointCamera; }
    double windowAspect() const { return m_windowAspect; }
    const Core::Matrix4x4& projectionMatrix() const { return m_projectionMatrix; }
    const std::array<int, 4>& viewportTransform() const { return m_viewportTransform; }

    void setViewportTransform(const std::array<int, 4>& values)
    {
        std::copy(values.cbegin(), values.cend(), m_viewportTransform.begin());
    }

    void activate(Model::Scene* scene);
    void deactivate();
    void adjustProjection(int width, int height);
    std::vector<const Graphics::GraphicsObject*> probe(int x, int y) const;

    // Needs Graphics context
    void loadShaders();
    void initializeAxisLegend();
    void render() const;

signals:
    void viewChanged();

private: // slots
    void add(Geometry::Geometry* geometry);
    void handleExtentChanged();
    void emitViewChanged() { emit viewChanged(); } // needed for connecting Nano signal to Qt signal

private:
    void updateProjectionMatrix();

    std::unique_ptr<Geometry::SceneGeometry> m_sceneGeometry{};
    std::vector<GraphicsObject*> m_graphicsObjectList{};

    std::unique_ptr<ShaderProgram> m_shaderProgram{};

    std::array<float, 4> m_ambientLight{{0.1f, 0.1f, 0.1f, 1.0f}};

    std::array<float, 4> m_light0Position{{0.0f, 0.0f, 1.0f, 1.0f}};
    std::array<float, 4> m_light0DiffuseColor{{0.7f, 0.7f, 0.7f, 1.0f}};
    std::array<float, 4> m_light0SpecularColor{{0.4f, 0.4f, 0.4f, 1.0f}};

    std::unique_ptr<Model::Viewpoint> m_defaultViewpoint{};
    ViewpointCamera m_viewpointCamera;

    double m_windowAspect{1.6};
    Core::Matrix4x4 m_projectionMatrix;

    std::array<int, 4> m_viewportTransform;

    std::unique_ptr<AxisLegend> m_axisLegend{};

    static const char* kVertexShaderSource;
    static const char* kFragmentShaderSource;
};

} // namespace Graphics

#endif // GRAPHICS_GRAPHICSENVIRONMENT_H
