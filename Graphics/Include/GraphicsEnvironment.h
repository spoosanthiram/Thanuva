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

#include "GeometryContainer.h"
#include "GraphicsObject.h"
#include "ShaderProgram.h"
#include "ViewpointCamera.h"

namespace Graphics {

class GraphicsEnvironment : public QObject
{
    Q_OBJECT

public:
    static const double kExtentMultiplier;

public:
    GraphicsEnvironment();

    const std::unique_ptr<Geometry::GeometryContainer>& geometryContainer() const
    {
        return m_geometryContainer;
    }
    const std::unique_ptr<ShaderProgram>& shaderProgram() const { return m_shaderProgram; }
    const ViewpointCamera& viewpointCamera() const { return m_viewpointCamera; }
    ViewpointCamera& viewpointCamera() { return m_viewpointCamera; }
    const Core::Matrix4x4& projectionMatrix() const { return m_projectionMatrix; }
    const std::array<int, 4>& viewportTransform() const { return m_viewportTransform; }

    void setViewportTransform(const std::array<int, 4>& values)
    {
        std::copy(values.cbegin(), values.cend(), m_viewportTransform.begin());
    }

    void activate(Model::Project* project);
    void deactivate();
    void adjustProjection(int width, int height);
    std::vector<const Graphics::GraphicsObject*> probe(int x, int y) const;

    // Needs Graphics context
    void loadShaders();
    void render() const;

signals:
    void viewChanged();

protected: // slots
    void add(Geometry::GeometryObject& graphicsObject);
    void handleExtentChanged();
    void emitViewChanged() { emit viewChanged(); } // needed for connecting Nano signal to Qt signal

private:
    void updateProjectionMatrix();

    std::unique_ptr<Geometry::GeometryContainer> m_geometryContainer{};
    std::vector<GraphicsObject*> m_graphicsObjectList{};

    std::unique_ptr<ShaderProgram> m_shaderProgram{};

    std::array<float, 4> m_ambientLight{{0.1f, 0.1f, 0.1f, 1.0f}};

    std::array<float, 4> m_light0Position{{0.0f, 0.0f, 1.0f, 1.0f}};
    std::array<float, 4> m_light0DiffuseColor{{0.7f, 0.7f, 0.7f, 1.0f}};
    std::array<float, 4> m_light0SpecularColor{{0.4f, 0.4f, 0.4f, 1.0f}};

    ViewpointCamera m_viewpointCamera{};

    double m_windowAspect{1.6};
    Core::Matrix4x4 m_projectionMatrix;

    std::array<int, 4> m_viewportTransform;

    static const char* kVertexShaderSource;
    static const char* kFragmentShaderSource;
};

} // namespace Graphics

#endif // GRAPHICS_GRAPHICSENVIRONMENT_H
