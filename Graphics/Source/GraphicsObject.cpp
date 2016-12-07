/*
 * Graphics: Graphical objects for Thanuva Model/Geometry obejcts
 *
 * Copyright 2016, Saravanan Poosanthiram
 * All rights reserved.
 */

#include "GraphicsObject.h"

#include <GL/glu.h>
#include <glog/logging.h>

#include "GeometryObject.h"
#include "GraphicsEnvironment.h"
#include "Matrix3x3.h"
#include "ModelObject.h"
#include "ViewpointCamera.h"

namespace Graphics {

GraphicsObject::GraphicsObject(const GraphicsEnvironment& graphicsEnvironment,
                               Geometry::GeometryObject* geometryObject)
    : m_graphicsEnvironment{graphicsEnvironment}
    , m_geometryObject{geometryObject}
{
    CHECK(geometryObject);
    LOG(INFO) << "Creating VAO and buffers for vertex, normal and index data.";

    g_OpenGLFuncs->glGenVertexArrays(1, &m_vaoHandle);

    g_OpenGLFuncs->glGenBuffers(kBufferSize, m_bufferHandle);

    this->initialize();

    m_geometryObject->geometryObjectChanged.connect<GraphicsObject, &GraphicsObject::initialize>(this);
}

void GraphicsObject::render() const
{
    float matrixData[16]; // for passing to opengl (both 3x3 & 4x4 matrix)
    auto& shaderProgram = m_graphicsEnvironment.shaderProgram();

    Core::Matrix4x4 modelViewMatrix = m_graphicsEnvironment.viewpointCamera().viewMatrix()
                                        /** m_geometryObject.transformMatrix()*/;
    modelViewMatrix.data(matrixData);
    g_OpenGLFuncs->glUniformMatrix4fv(shaderProgram->modelViewMatrixLocation(), 1, GL_FALSE, matrixData);

    Core::Matrix3x3 normalMatrix = modelViewMatrix;
    normalMatrix.invert();
    normalMatrix.transpose();
    normalMatrix.data(matrixData);
    g_OpenGLFuncs->glUniformMatrix3fv(shaderProgram->normalMatrixLocation(), 1, GL_FALSE, matrixData);

    const Core::Material& material = m_geometryObject->modelObject()->material();

    const Core::Color& diffuseColor = material.diffuseColor();
    g_OpenGLFuncs->glUniform4f(shaderProgram->diffuseColorLocation(),
                               diffuseColor.r(), diffuseColor.g(), diffuseColor.b(), 1.0f);

    const Core::Color& specularColor = material.specularColor();
    g_OpenGLFuncs->glUniform4f(shaderProgram->specularColorLocation(),
                               specularColor.r(), specularColor.g(), specularColor.b(), 1.0f);

    g_OpenGLFuncs->glUniform1f(shaderProgram->shininessLocation(), material.shininess());

    g_OpenGLFuncs->glBindVertexArray(m_vaoHandle);
    const std::vector<int>& indices = m_geometryObject->indices();
    g_OpenGLFuncs->glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indices.size()),
                                      GL_UNSIGNED_INT, 0);
}

bool GraphicsObject::probe(int x, int y) const
{
    Core::Vector3d nearPoint = this->glNearPoint(x, y);
    Core::Vector3d farPoint = this->glFarPoint(x, y);

    return m_geometryObject->intersectBoundingBox(nearPoint, farPoint)
            && m_geometryObject->intersect(nearPoint, farPoint, nullptr);
}

std::vector<Core::Vector3d> GraphicsObject::probePoints(int x, int y) const
{
    std::vector<Core::Vector3d> probePoints{};
    return probePoints;
}

void GraphicsObject::initialize()
{
    g_OpenGLFuncs->glBindVertexArray(m_vaoHandle);

    LOG(INFO) << "Binding vertex buffer, filling vertex data and using vertex shader location 0.";

    g_OpenGLFuncs->glBindBuffer(GL_ARRAY_BUFFER, m_bufferHandle[kVertexBuffer]);
    const std::vector<float>& vertices = m_geometryObject->vertices();
    g_OpenGLFuncs->glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float),
                                vertices.data(), GL_STATIC_DRAW);
    g_OpenGLFuncs->glVertexAttribPointer(kVertexLocation,
            Geometry::GeometryObject::kValuesPerVertex, GL_FLOAT, GL_FALSE, 0, 0);
    g_OpenGLFuncs->glEnableVertexAttribArray(kVertexLocation);

    LOG(INFO) << "Binding normal buffer, filling normal data and using vertex shader location 1.";

    g_OpenGLFuncs->glBindBuffer(GL_ARRAY_BUFFER, m_bufferHandle[kNormalBuffer]);
    const std::vector<float>& normals = m_geometryObject->normals();
    g_OpenGLFuncs->glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(float),
                                normals.data(), GL_STATIC_DRAW);
    g_OpenGLFuncs->glVertexAttribPointer(kNormalLocation,
            Geometry::GeometryObject::kValuesPerVertex, GL_FLOAT, GL_FALSE, 0, 0);
    g_OpenGLFuncs->glEnableVertexAttribArray(kNormalLocation);

    const std::vector<int>& indices = m_geometryObject->indices();

    LOG(INFO) << "Binding index buffer, filling index data.";

    g_OpenGLFuncs->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_bufferHandle[kIndexBuffer]);
    g_OpenGLFuncs->glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(int),
                                indices.data(), GL_STATIC_DRAW);

    graphicsObjectChanged.emit_signal(); // emit signal
}

Core::Vector3d GraphicsObject::glNearPoint(int x, int y) const
{
    Core::Matrix4x4 modelViewMatrix = m_graphicsEnvironment.viewpointCamera().viewMatrix()
                                        /** m_geometryObject.transformMatrix()*/;
    Core::Matrix4x4 projectionMatrix = m_graphicsEnvironment.projectionMatrix();
    double nearPoint[3];
    gluUnProject(x, y, 0.0, modelViewMatrix.data(), projectionMatrix.data(),
                 m_graphicsEnvironment.viewportTransform().data(),
                 &nearPoint[0], &nearPoint[1], &nearPoint[2]);
    return Core::Vector3d(nearPoint);
}

Core::Vector3d GraphicsObject::glFarPoint(int x, int y) const
{
    Core::Matrix4x4 modelViewMatrix = m_graphicsEnvironment.viewpointCamera().viewMatrix()
                                        /** m_geometryObject.transformMatrix()*/;
    Core::Matrix4x4 projectionMatrix = m_graphicsEnvironment.projectionMatrix();
    double farPoint[3];
    gluUnProject(x, y, 1.0, modelViewMatrix.data(), projectionMatrix.data(),
                 m_graphicsEnvironment.viewportTransform().data(),
                 &farPoint[0], &farPoint[1], &farPoint[2]);
    return Core::Vector3d(farPoint);
}

} // namespace Graphics
