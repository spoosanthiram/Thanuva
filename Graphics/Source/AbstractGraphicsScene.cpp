/*
 * Graphics: Graphical objects for Thanuva Model/Geometry obejcts
 *
 * Copyright 2016, Saravanan Poosanthiram
 * All rights reserved.
 */

#include "AbstractGraphicsScene.h"

#include <glog/logging.h>

namespace Graphics {

const char* AbstractGraphicsScene::kProjectionMatrixUniformName = "projectionMatrix";
const char* AbstractGraphicsScene::kModelViewMatrixUniformName = "modelViewMatrix";
const char* AbstractGraphicsScene::kNormalMatrixUniformName = "normalMatrix";

const char* AbstractGraphicsScene::kAmbientLightUniformName = "ambientLight";

const char* AbstractGraphicsScene::kLight0PositionUniformName = "light0Position";
const char* AbstractGraphicsScene::kLight0DiffuseColorUniformName = "light0DiffuseColor";
const char* AbstractGraphicsScene::kLight0SpecularColorUniformName = "light0SpecularColor";

const char* AbstractGraphicsScene::kDiffuseColorUniformName = "diffuseColor";
const char* AbstractGraphicsScene::kSpecularColorUniformName = "specularColor";
const char* AbstractGraphicsScene::kShininessUniformName = "shininess";

AbstractGraphicsScene::AbstractGraphicsScene()
    : m_viewpointCamera{*this}
    , m_projectionMatrix{Core::Matrix4x4::identity()}
{
    m_viewpointCamera.viewpointCameraChanged.connect<AbstractGraphicsScene, &AbstractGraphicsScene::emitGraphicsSceneChanged>(this);
}

void AbstractGraphicsScene::loadShaders()
{
    try {
        LOG(INFO) << "Creating ShaderProgram.";
        m_shaderProgram = std::make_unique<ShaderProgram>();

        LOG(INFO) << "Creating Vertex Shader, compiling and attaching to ShaderProgram.";
        Shader vertexShader{GL_VERTEX_SHADER};
        vertexShader.compile(this->vertexShaderSource());

        m_shaderProgram->attachShader(vertexShader);

        LOG(INFO) << "Creating Fragment Shader, compiling and attaching to ShaderProgram.";
        Shader fragmentShader{GL_FRAGMENT_SHADER};
        fragmentShader.compile(this->fragmentShaderSource());

        m_shaderProgram->attachShader(fragmentShader);

        LOG(INFO) << "Linking shaders and updating uniform locations.";
        m_shaderProgram->link();
    }
    catch (const std::exception& e) {
        LOG(ERROR) << e.what();
        throw;
    }

    m_projectionMatrixLocation = g_OpenGLFuncs->glGetUniformLocation(m_shaderProgram->handle(), kProjectionMatrixUniformName);
    m_modelViewMatrixLocation = g_OpenGLFuncs->glGetUniformLocation(m_shaderProgram->handle(), kModelViewMatrixUniformName);
    m_normalMatrixLocation = g_OpenGLFuncs->glGetUniformLocation(m_shaderProgram->handle(), kNormalMatrixUniformName);

    m_ambientLightLocation = g_OpenGLFuncs->glGetUniformLocation(m_shaderProgram->handle(), kAmbientLightUniformName);

    this->updateShaderLocations();
}

void AbstractGraphicsScene::handleWindowResize(int width, int height)
{
    m_windowAspect = static_cast<double>(width) / static_cast<double>(height);
    this->handleWindowAspectChanged();
}

void AbstractGraphicsScene::add(Geometry::Geometry* geometry)
{
    LOG(INFO) << "Adding Geometry::Geometry into viewObjectList.";

    auto graphicsObject = std::make_unique<GraphicsObject>(*this, geometry);
    auto rawPtr = graphicsObject.get();
    m_graphicsObjectList.push_back(std::move(graphicsObject));
    rawPtr->graphicsObjectChanged.connect<AbstractGraphicsScene, &AbstractGraphicsScene::emitGraphicsSceneChanged>(this);

    this->emitGraphicsSceneChanged(); // emit signal
}

} // namespace Graphics
