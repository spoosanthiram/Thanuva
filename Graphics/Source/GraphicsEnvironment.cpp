/*
 * Graphics: Graphical objects for Thanuva Model/Geometry obejcts
 *
 * Copyright 2016, Saravanan Poosanthiram
 * All rights reserved.
 */

#include "GraphicsEnvironment.h"

#include <glog/logging.h>
#include <nano_signal_slot.hpp>

#include "GraphicsObject.h"
#include "Project.h"

namespace Graphics {

const double GraphicsEnvironment::kExtentMultiplier = 3.0;

GraphicsEnvironment::GraphicsEnvironment()
    : m_projectionMatrix{Core::Matrix4x4::identity()}
{
}

void GraphicsEnvironment::activate(Model::Project* project)
{
    CHECK(!m_geometryContainer) << "GraphicsEnvironment::activate: old one still active!";

    LOG(INFO) << "Activating GraphicsEnvironment with Model::Project.";

    m_geometryContainer = std::make_unique<Geometry::GeometryContainer>(project);

    for (auto geometryObject : m_geometryContainer->geometryObjectList())
        this->add(*geometryObject);
    m_viewpointCamera.setViewpoint(project->viewpoint());

    this->handleExtentChanged();

    m_geometryContainer->geometryObjectAdded.
            connect<GraphicsEnvironment, &GraphicsEnvironment::add>(this);
    m_geometryContainer->extentChanged.
            connect<GraphicsEnvironment, &GraphicsEnvironment::handleExtentChanged>(this);
    m_viewpointCamera.viewpointCameraChanged.
            connect<GraphicsEnvironment, &GraphicsEnvironment::emitViewChanged>(this);
}

void GraphicsEnvironment::deactivate()
{
    LOG(INFO) << "Deactivating GraphicsEnvironment.";

    m_viewpointCamera.viewpointCameraChanged.
            disconnect<GraphicsEnvironment, &GraphicsEnvironment::emitViewChanged>(this);
    m_geometryContainer->geometryObjectAdded.
            disconnect<GraphicsEnvironment, &GraphicsEnvironment::add>(this);
    m_geometryContainer->extentChanged.
            disconnect<GraphicsEnvironment, &GraphicsEnvironment::handleExtentChanged>(this);

    m_geometryContainer.reset(nullptr);
}

void GraphicsEnvironment::adjustProjection(int width, int height)
{
    m_windowAspect = static_cast<double>(width) / static_cast<double>(height);
    this->updateProjectionMatrix();
}

std::vector<const Graphics::GraphicsObject*> GraphicsEnvironment::probe(int x, int y) const
{
    std::vector<const Graphics::GraphicsObject*> result{};
    for (const auto& graphicsObject : m_graphicsObjectList) {
        if (graphicsObject->probe(x, y))
            result.push_back(graphicsObject);
    }
    return result;
}

void GraphicsEnvironment::loadShaders()
{
    try {
        LOG(INFO) << "Creating ShaderProgram.";
        m_shaderProgram = std::make_unique<ShaderProgram>();

        LOG(INFO) << "Creating Vertex Shader, compiling and attaching to ShaderProgram.";
        Shader vertexShader{GL_VERTEX_SHADER};
        vertexShader.compile(kVertexShaderSource);

        m_shaderProgram->attachShader(vertexShader);

        LOG(INFO) << "Creating Fragment Shader, compiling and attaching to ShaderProgram.";
        Shader fragmentShader{GL_FRAGMENT_SHADER};
        fragmentShader.compile(kFragmentShaderSource);

        m_shaderProgram->attachShader(fragmentShader);

        LOG(INFO) << "Linking shaders and updating uniform locations.";
        m_shaderProgram->link();
    }
    catch (const std::exception& e) {
        LOG(ERROR) << e.what();
        throw;
    }
}

void GraphicsEnvironment::render() const
{
    if (!m_geometryContainer)
        return;

    g_OpenGLFuncs->glUseProgram(m_shaderProgram->handle());

    g_OpenGLFuncs->glUniform4fv(m_shaderProgram->ambientLightLocation(), 1, m_ambientLight.data());

    g_OpenGLFuncs->glUniform4fv(m_shaderProgram->light0PositionLocation(), 1,
                                m_light0Position.data());
    g_OpenGLFuncs->glUniform4fv(m_shaderProgram->light0DiffuseColorLocation(), 1,
                                m_light0DiffuseColor.data());
    g_OpenGLFuncs->glUniform4fv(m_shaderProgram->light0SpecularColorLocation(), 1,
                                m_light0SpecularColor.data());

    float matrixData[16]; // 4x4 matrix
    const Core::Matrix4x4& projectionMatrix = m_projectionMatrix;
    projectionMatrix.data(matrixData);
    g_OpenGLFuncs->glUniformMatrix4fv(m_shaderProgram->projectionMatrixLocation(), 1,
                                      GL_FALSE, matrixData);

    for (const auto& graphicsObject : m_graphicsObjectList)
        graphicsObject->render();
}

void GraphicsEnvironment::add(Geometry::GeometryObject& geometryObject)
{
    LOG(INFO) << "Adding GfxModel::GeometryObject into viewObjectList.";

    GraphicsObject* viewObject = new GraphicsObject{*this, geometryObject};
    m_graphicsObjectList.push_back(viewObject);
    m_graphicsObjectList.back()->graphicsObjectChanged.
            connect<GraphicsEnvironment, &GraphicsEnvironment::emitViewChanged>(this);

    emit viewChanged();
}

void GraphicsEnvironment::handleExtentChanged()
{
    CHECK(m_geometryContainer);

    m_light0Position[1] = m_geometryContainer->extent().maxLength();
    m_light0Position[2] = m_geometryContainer->extent().maxLength() * kExtentMultiplier;

    m_viewpointCamera.setViewpointTranslation(
            m_geometryContainer->extent().maxLength() * kExtentMultiplier);
    this->updateProjectionMatrix();

    this->emitViewChanged();
}

void GraphicsEnvironment::updateProjectionMatrix()
{
    if (!m_geometryContainer)
        return;

    m_projectionMatrix = Core::Matrix4x4::perspective(50.0, m_windowAspect, 0.1,
            m_geometryContainer->extent().maxLength() * kExtentMultiplier);
}

const char* GraphicsEnvironment::kVertexShaderSource =
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


const char* GraphicsEnvironment::kFragmentShaderSource =
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

    vec3 eyeDir = normalize(eyePos - vertexPos);
    vec3 normalVec = normalize(normal);

    vec3 lightDir = normalize(light0Position.xyz - vertexPos);
    vec3 halfVec = normalize(lightDir + eyeDir);

    vec4 lambert = light0DiffuseColor * diffuseColor * max(dot(normalVec, lightDir), 0.0);
    vec4 phong = light0SpecularColor * specularColor * pow(max(dot(normalVec, halfVec), 0.0), shininess);

    color = min(ambientLight + lambert + phong, vec4(1.0));
}
)FRAG";

} // namespace Graphics
