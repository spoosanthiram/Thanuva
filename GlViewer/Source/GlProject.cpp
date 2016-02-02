/**
 * Source file Viewer Project class.
 *
 * Author: Saravanan Poosanthiram
 */

#include "GlProject.h"

#include <glog/logging.h>
#include <nano_signal_slot.hpp>
#include <QOpenGLFunctions_4_3_Core>

#include "GlViewObject.h"

namespace GlViewer {

GlProject::GlProject()
    : m_gfxProject{}
    , m_glViewObjectList{}
    , m_glFuncsPtr{nullptr}
    , m_shaderProgram{}
    , m_glViewportTransform{}
{
    m_ambientLight[0] = 0.1f; m_ambientLight[1] = 0.1f; m_ambientLight[2] = 0.1f; m_ambientLight[3] = 1.0f;

    m_light0Position[0] = 0.0f; m_light0Position[1] = 0.0f; m_light0Position[2] = 1.0f; m_light0Position[3] = 1.0f;
    m_light0DiffuseColor[0] = 0.7f; m_light0DiffuseColor[1] = 0.7f; m_light0DiffuseColor[2] = 0.7f; m_light0DiffuseColor[3] = 1.0f;
    m_light0SpecularColor[0] = 0.4f; m_light0SpecularColor[1] = 0.4f; m_light0SpecularColor[2] = 0.4f; m_light0SpecularColor[3] = 1.0f;
}

void GlProject::activate(Model::Project* project)
{
    CHECK(!m_gfxProject) << "GlProject::activate: old one still active!";

    LOG(INFO) << "Activating GlProject with Model::Project.";

    m_gfxProject = std::make_unique<GfxModel::GfxProject>(project);

    for (auto graphicsObject : m_gfxProject->graphicsObjectList())
        this->add(*graphicsObject);

    this->handleExtentChanged();

    m_gfxProject->graphicsObjectAdded.connect<GlProject, &GlProject::add>(this);
    m_gfxProject->extentChanged.connect<GlProject, &GlProject::handleExtentChanged>(this);
    m_gfxProject->viewpointCamera().viewpointCameraChanged.connect<GlProject, &GlProject::emitViewChanged>(this);
}

void GlProject::deactivate()
{
    LOG(INFO) << "Deactivating GlProject.";

    m_gfxProject->viewpointCamera().viewpointCameraChanged.disconnect<GlProject, &GlProject::emitViewChanged>(this);
    m_gfxProject->graphicsObjectAdded.disconnect<GlProject, &GlProject::add>(this);
    m_gfxProject->extentChanged.disconnect<GlProject, &GlProject::handleExtentChanged>(this);

    m_gfxProject.reset(nullptr);
}

void GlProject::loadShaders()
{
    try {
        LOG(INFO) << "Creating ShaderProgram.";
        m_shaderProgram = std::make_unique<ShaderProgram>(this);

        LOG(INFO) << "Creating Vertex Shader, compiling and attaching to ShaderProgram.";
        Shader vertexShader{this, GL_VERTEX_SHADER};
        vertexShader.compile(kVertexShaderSource);

        m_shaderProgram->attachShader(vertexShader);

        LOG(INFO) << "Creating Fragment Shader, compiling and attaching to ShaderProgram.";
        Shader fragmentShader{this, GL_FRAGMENT_SHADER};
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

void GlProject::render() const
{
    if (!m_gfxProject)
        return;

    m_glFuncsPtr->glUseProgram(m_shaderProgram->handle());

    m_glFuncsPtr->glUniform4fv(m_shaderProgram->ambientLightLocation(), 1, m_ambientLight);

    m_glFuncsPtr->glUniform4fv(m_shaderProgram->light0PositionLocation(), 1, m_light0Position);
    m_glFuncsPtr->glUniform4fv(m_shaderProgram->light0DiffuseColorLocation(), 1, m_light0DiffuseColor);
    m_glFuncsPtr->glUniform4fv(m_shaderProgram->light0SpecularColorLocation(), 1, m_light0SpecularColor);

    float matrixData[16]; // 4x4 matrix
    const Core::Matrix4x4& projectionMatrix = m_gfxProject->projectionMatrix();
    projectionMatrix.data(matrixData);
    m_glFuncsPtr->glUniformMatrix4fv(m_shaderProgram->projectionMatrixLocation(), 1, GL_FALSE, matrixData);

    for (const auto& glViewObject : m_glViewObjectList)
        glViewObject->render();
}

std::vector<const GlViewer::GlViewObject*> GlProject::probe(int x, int y) const
{
    std::vector<const GlViewer::GlViewObject*> result{};
    for (const auto& glViewObject : m_glViewObjectList) {
        if (glViewObject->probe(x, y))
            result.push_back(glViewObject);
    }
    return result;
}

void GlProject::add(GfxModel::GraphicsObject& graphicsObject)
{
    LOG(INFO) << "Adding GfxModel::GraphicsObject into viewObjectList.";

    GlViewObject* viewObject = new GlViewObject{*this, graphicsObject};
    m_glViewObjectList.push_back(viewObject);
    m_glViewObjectList.back()->glViewObjectChanged.connect<GlProject, &GlProject::emitViewChanged>(this);

    emit viewChanged();
}

void GlProject::handleExtentChanged()
{
    CHECK(m_gfxProject);

    m_light0Position[1] = m_gfxProject->extent().maxLength();
    m_light0Position[2] = m_gfxProject->extent().maxLength() * 2.0;
}


const char* GlProject::kVertexShaderSource =
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


const char* GlProject::kFragmentShaderSource =
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

} // namespace GlViewer
