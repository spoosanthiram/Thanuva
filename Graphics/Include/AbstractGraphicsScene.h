/*
 * Graphics: Graphical objects for Thanuva Model/Geometry obejcts
 *
 * Copyright 2016, Saravanan Poosanthiram
 * All rights reserved.
 */

#ifndef Graphics_AbstractGraphicsScene_h
#define Graphics_AbstractGraphicsScene_h

#include <memory>

#include <nano_signal_slot.hpp>

#include "GraphicsObject.h"
#include "ShaderProgram.h"
#include "ViewpointCamera.h"

namespace Geometry { class Geometry; }

namespace Graphics {

class AbstractGraphicsScene
{
public:
    static const char* kProjectionMatrixUniformName;
    static const char* kModelViewMatrixUniformName;
    static const char* kNormalMatrixUniformName;

    static const char* kAmbientLightUniformName;

    static const char* kLight0PositionUniformName;
    static const char* kLight0DiffuseColorUniformName;
    static const char* kLight0SpecularColorUniformName;

    static const char* kDiffuseColorUniformName;
    static const char* kSpecularColorUniformName;
    static const char* kShininessUniformName;

public:
    AbstractGraphicsScene();
    virtual ~AbstractGraphicsScene() {}

    const std::vector<std::unique_ptr<GraphicsObject>>& graphicsObjectList() const { return m_graphicsObjectList; }
    const std::unique_ptr<ShaderProgram>& shaderProgram() const { return m_shaderProgram; }
    const float* ambientLight() const { return m_ambientLight.data(); }
    const ViewpointCamera& viewpointCamera() const { return m_viewpointCamera; }
    ViewpointCamera& viewpointCamera() { return m_viewpointCamera; }
    double windowAspect() const { return m_windowAspect; }
    const Core::Matrix4x4& projectionMatrix() const { return m_projectionMatrix; }
    const std::array<int, 4>& viewportTransform() const { return m_viewportTransform; }

    GLint projectionMatrixLocation() const { return m_projectionMatrixLocation; }
    GLint modelViewMatrixLocation() const { return m_modelViewMatrixLocation; }
    GLint normalMatrixLocation() const { return m_normalMatrixLocation; }

    GLint ambientLightLocation() const { return m_ambientLightLocation; }

    virtual GLint diffuseColorLocation() const = 0;
    virtual bool isSpecularColorAvailable() const = 0;
    virtual GLint specularColorLocation() const = 0;
    virtual GLint shininessLocation() const = 0;

    void setProjectionMatrix(const Core::Matrix4x4& projMatrix) { m_projectionMatrix = projMatrix; }
    void setViewportTransform(const std::array<int, 4>& values)
    {
        std::copy(values.cbegin(), values.cend(), m_viewportTransform.begin());
    }

    void loadShaders();
    void handleWindowResize(int width, int height);

    virtual std::string vertexShaderSource() = 0;
    virtual std::string fragmentShaderSource() = 0;
    virtual void updateShaderLocations() = 0;
    virtual void handleWindowAspectChanged() = 0;
    virtual void render() const = 0;

public: // signals
    Nano::Signal<void()> graphicsSceneChanged{};

public: // slots
    void add(Geometry::Geometry* geometry);

protected: // slots
    void emitGraphicsSceneChanged() { graphicsSceneChanged.emit_signal(); }

private:
    std::vector<std::unique_ptr<GraphicsObject>> m_graphicsObjectList{};

    std::unique_ptr<ShaderProgram> m_shaderProgram{};
    std::array<float, 4> m_ambientLight{{0.1f, 0.1f, 0.1f, 1.0f}};

    ViewpointCamera m_viewpointCamera;

    double m_windowAspect{1.6};
    Core::Matrix4x4 m_projectionMatrix;

    std::array<int, 4> m_viewportTransform;

    GLint m_projectionMatrixLocation{-1};
    GLint m_modelViewMatrixLocation{-1};
    GLint m_normalMatrixLocation{-1};

    GLint m_ambientLightLocation{-1};
};

} // namespace Graphics

#endif // Graphics_AbstractGraphicsScene_h
