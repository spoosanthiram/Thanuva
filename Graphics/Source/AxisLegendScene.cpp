/*
 * Graphics: Graphical objects for Thanuva Model/Geometry obejcts
 *
 * Copyright 2016, Saravanan Poosanthiram
 * All rights reserved.
 */

#include "AxisLegendScene.h"

#include "ConeModel.h"
#include "CylinderModel.h"
#include "GeometryScene.h"
#include "Scene.h"

namespace Graphics {

const double AxisLegendScene::kLegendFarProjection = 10.0;

AxisLegendScene::AxisLegendScene(const Model::ThanuvaApp& app)
    : AbstractGraphicsScene{}
{
    m_scene = std::make_unique<Model::Scene>(app);

    // X Axis
    auto xCylinderModel = m_scene->newModelObject<Model::CylinderModel>(Core::Point3d{0.0, 0.0, 0.0}, 0.025,
                                                                        Core::Point3d{0.25, 0.0, 0.0}, 0.025, 32);
    xCylinderModel->setMaterial({Core::Color{255, 0, 0}, Core::Color{255, 255, 255}, 128.0f});
    auto xConeModel = m_scene->newModelObject<Model::ConeModel>(Core::Point3d{0.35, 0.0, 0.0},
                                                                Core::Point3d{0.25, 0.0, 0.0}, 0.05, 32);
    xConeModel->setMaterial({Core::Color{255, 0, 0}, Core::Color{255, 255, 255}, 128.0f});

    // Y Axis
    auto yCylinderModel = m_scene->newModelObject<Model::CylinderModel>(Core::Point3d{0.0, 0.0, 0.0}, 0.025,
                                                                        Core::Point3d{0.0, 0.25, 0.0}, 0.025, 32);
    yCylinderModel->setMaterial({Core::Color{0, 255, 0}, Core::Color{255, 255, 255}, 128.0f});
    auto yConeModel = m_scene->newModelObject<Model::ConeModel>(Core::Point3d{0.0, 0.35, 0.0},
                                                                Core::Point3d{0.0, 0.25, 0.0}, 0.05, 32);
    yConeModel->setMaterial({Core::Color{0, 255, 0}, Core::Color{255, 255, 255}, 128.0f});

    // Z Axis
    auto zCylinderModel = m_scene->newModelObject<Model::CylinderModel>(Core::Point3d{0.0, 0.0, 0.0}, 0.025,
                                                                        Core::Point3d{0.0, 0.0, 0.25}, 0.025, 32);
    zCylinderModel->setMaterial({Core::Color{0, 0, 255}, Core::Color{255, 255, 255}, 128.0f});
    auto zConeModel = m_scene->newModelObject<Model::ConeModel>(Core::Point3d{0.0, 0.0, 0.35},
                                                                Core::Point3d{0.0, 0.0, 0.25}, 0.05, 32);
    zConeModel->setMaterial({Core::Color{0, 0, 255}, Core::Color{255, 255, 255}, 128.0f});
}

void AxisLegendScene::activateScene()
{
    m_geometryScene = std::make_unique<Geometry::GeometryScene>(m_scene.get());

    for (auto& geometry : m_geometryScene->geometryObjectList())
        this->add(geometry.get());

    this->viewpointCamera().setViewpoint(m_scene->viewpointList().front().get());
    this->viewpointCamera().setViewpointTranslation(AxisLegendScene::kLegendFarProjection / 2.0);
}

std::string AxisLegendScene::vertexShaderSource()
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

std::string AxisLegendScene::fragmentShaderSource()
{
    return
R"FRAG(
#version 430 core

in vec4 vertex;
in vec3 normal;

uniform vec4 ambientLight;

uniform vec4 light0Position;
uniform vec4 light0DiffuseColor;

uniform vec4 diffuseColor;

out vec4 color;

void main()
{
    vec3 vertexPos = vertex.xyz / vertex.z;
    vec3 normalVec = normalize(normal);
    vec3 lightDir = normalize(light0Position.xyz - vertexPos);

    vec4 lambert = light0DiffuseColor * diffuseColor * max(dot(normalVec, lightDir), 0.0);

    color = min(ambientLight + lambert, vec4(1.0));
}
)FRAG";
}

void AxisLegendScene::updateShaderLocations()
{
    auto handle = this->shaderProgram()->handle();

    m_light0PositionLocation = g_OpenGLFuncs->glGetUniformLocation(handle, kLight0PositionUniformName);
    m_light0DiffuseColorLocation = g_OpenGLFuncs->glGetUniformLocation(handle, kLight0DiffuseColorUniformName);

    m_diffuseColorLocation = g_OpenGLFuncs->glGetUniformLocation(handle, kDiffuseColorUniformName);
}

void AxisLegendScene::handleWindowAspectChanged()
{
    double halfFar = AxisLegendScene::kLegendFarProjection / 2.0;
    Core::Point3d p{-(halfFar * this->windowAspect() - halfFar * 0.1), -(halfFar - halfFar * 0.1), 0.0};
    this->viewpointCamera().translate(p);

    double halfLen = AxisLegendScene::kLegendFarProjection / 2.0;
    this->setProjectionMatrix(Core::Matrix4x4::ortho(-halfLen * this->windowAspect(), halfLen * this->windowAspect(),
                                                     -halfLen, halfLen, 0.0, AxisLegendScene::kLegendFarProjection));
}

void AxisLegendScene::render() const
{
    g_OpenGLFuncs->glUseProgram(this->shaderProgram()->handle());

    g_OpenGLFuncs->glUniform4fv(this->ambientLightLocation(), 1, this->ambientLight());

    g_OpenGLFuncs->glUniform4fv(m_light0PositionLocation, 1, m_light0Position.data());
    g_OpenGLFuncs->glUniform4fv(m_light0DiffuseColorLocation, 1, m_light0DiffuseColor.data());

    float matrixData[16]; // 4x4 matrix
    this->projectionMatrix().data(matrixData);
    g_OpenGLFuncs->glUniformMatrix4fv(this->projectionMatrixLocation(), 1, GL_FALSE, matrixData);

    for (auto& graphicsObject : this->graphicsObjectList())
        graphicsObject->render();
}

} // namespace Graphics
