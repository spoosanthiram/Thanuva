/*
 * Graphics: Graphical objects for Thanuva Model/Geometry obejcts
 *
 * Copyright 2016, Saravanan Poosanthiram
 * All rights reserved.
 */

#include "GraphicsScene.h"

#include <glog/logging.h>

#include "Scene.h"

namespace Graphics {

const double GraphicsScene::kViewpointTranslationMultiplier = 2.5;
const double GraphicsScene::kFarProjectionMultiplier = 7.0;

GraphicsScene::GraphicsScene()
    : AbstractGraphicsScene{}
{
}

void GraphicsScene::activate(Model::Scene* scene)
{
    CHECK(!m_geometryScene); // Make sure old scene is deactivated

    LOG(INFO) << "Activating GraphicsScene with Model::Scene: " << scene->name();

    m_geometryScene = std::make_unique<Geometry::GeometryScene>(scene);

    for (auto& geometry : m_geometryScene->geometryObjectList())
        this->add(geometry.get());

    this->viewpointCamera().setViewpoint(scene->viewpointList().front().get());

    this->handleExtentChanged();

    m_geometryScene->geometryAdded.connect<AbstractGraphicsScene, &AbstractGraphicsScene::add>(this);
    m_geometryScene->extentChanged.connect<GraphicsScene, &GraphicsScene::handleExtentChanged>(this);
}

void GraphicsScene::deactivate()
{
    LOG(INFO) << "Deactivating GraphicsEnvironment.";

    m_geometryScene->geometryAdded.disconnect<AbstractGraphicsScene, &AbstractGraphicsScene::add>(this);
    m_geometryScene->extentChanged.disconnect<GraphicsScene, &GraphicsScene::handleExtentChanged>(this);

    m_geometryScene.release();
}

std::vector<const Graphics::GraphicsObject*> GraphicsScene::probe(int x, int y) const
{
    std::vector<const Graphics::GraphicsObject*> result{};
    for (auto& graphicsObject : this->graphicsObjectList()) {
        if (graphicsObject->probe(x, y))
            result.push_back(graphicsObject.get());
    }
    return result;
}

std::string GraphicsScene::vertexShaderSource()
{
    return
R"VERT(
#version 430 core

layout(location = 0) in vec4 vertexPosition;
layout(location = 1) in vec3 vertexNormal;

uniform mat4 projectionMatrix;
uniform mat4 modelViewMatrix;
uniform mat3 normalMatrix;

out vec4 vertex;
out vec3 normal;

void main()
{
    vertex = modelViewMatrix * vertexPosition;
    normal = normalMatrix * vertexNormal;

    gl_Position = projectionMatrix * vertex;
}
)VERT";
}

std::string GraphicsScene::fragmentShaderSource()
{
    return
R"FRAG(
#version 430 core

in vec4 vertex;
in vec3 normal;

uniform vec4 ambientLight;

uniform vec4 light0Position;
uniform vec4 light0DiffuseColor;
uniform vec4 light0SpecularColor;

uniform vec4 diffuseColor;
uniform vec4 specularColor;
uniform float shininess;

out vec4 color;

void main()
{
    const vec3 eyePos = vec3(0.0, 0.0, 0.0);

    vec3 vertexPos = vertex.xyz / vertex.z;

    //vec3 eyeDir = normalize(eyePos - vertexPos);
    vec3 normalVec = normalize(normal);

    vec3 lightDir = normalize(light0Position.xyz - vertexPos);
    //vec3 halfVec = normalize(lightDir + eyeDir);

    vec4 lambert = light0DiffuseColor * diffuseColor * max(dot(normalVec, lightDir), 0.0);
    //vec4 phong = light0SpecularColor * specularColor * pow(max(dot(normalVec, halfVec), 0.0), shininess);

    color = min(ambientLight + lambert, vec4(1.0));
    //color = min(ambientLight + lambert + phong, vec4(1.0));
}
)FRAG";
}

void GraphicsScene::updateShaderLocations()
{
    auto handle = this->shaderProgram()->handle();

    m_light0PositionLocation = g_OpenGLFuncs->glGetUniformLocation(handle, kLight0PositionUniformName);
    m_light0DiffuseColorLocation = g_OpenGLFuncs->glGetUniformLocation(handle, kLight0DiffuseColorUniformName);
    m_light0SpecularColorLocation = g_OpenGLFuncs->glGetUniformLocation(handle, kLight0SpecularColorUniformName);

    m_diffuseColorLocation = g_OpenGLFuncs->glGetUniformLocation(handle, kDiffuseColorUniformName);
    m_specularColorLocation = g_OpenGLFuncs->glGetUniformLocation(handle, kSpecularColorUniformName);
    m_shininessLocation = g_OpenGLFuncs->glGetUniformLocation(handle, kShininessUniformName);
}

void GraphicsScene::render() const
{
    if (!m_geometryScene)
        return;

    g_OpenGLFuncs->glUseProgram(this->shaderProgram()->handle());

    g_OpenGLFuncs->glUniform4fv(this->ambientLightLocation(), 1, this->ambientLight());

    g_OpenGLFuncs->glUniform4fv(m_light0PositionLocation, 1, m_light0Position.data());
    g_OpenGLFuncs->glUniform4fv(m_light0DiffuseColorLocation, 1, m_light0DiffuseColor.data());
    g_OpenGLFuncs->glUniform4fv(m_light0SpecularColorLocation, 1, m_light0SpecularColor.data());

    float matrixData[16]; // 4x4 matrix
    this->projectionMatrix().data(matrixData);
    g_OpenGLFuncs->glUniformMatrix4fv(this->projectionMatrixLocation(), 1, GL_FALSE, matrixData);

    for (auto& graphicsObject : this->graphicsObjectList())
        graphicsObject->render();
}

void GraphicsScene::handleExtentChanged()
{
    CHECK(m_geometryScene);

    auto& extent = m_geometryScene->extent();
    if (extent.isAnyInfinite())
        return;

    m_light0Position[1] = extent.maxLength();
    m_light0Position[2] = extent.maxLength() * kViewpointTranslationMultiplier;

    this->viewpointCamera().setViewpointTranslation(extent.maxLength() * kViewpointTranslationMultiplier);
    this->updateProjectionMatrix();

    this->emitGraphicsSceneChanged();
}

void GraphicsScene::updateProjectionMatrix()
{
    if (!m_geometryScene)
        return;

    this->setProjectionMatrix(Core::Matrix4x4::perspective(
        60.0, this->windowAspect(), 0.1, m_geometryScene->extent().maxLength() * kFarProjectionMultiplier));
}

} // namespace Graphics
