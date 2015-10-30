/**
* Author: Saravanan Poosanthiram
* $LastChangedBy: ps $
* $LastChangedDate: 2015-03-31 17:41:12 -0400 (Tue, 31 Mar 2015) $
*/

#include "GlViewObject.h"

#include "glog/logging.h"

#include "GlProject.h"
#include "GraphicsObject.h"
#include "Matrix3x3.h"
#include "ViewpointCamera.h"

namespace GlViewer {

GlViewObject::GlViewObject(const GlProject& glProject, GfxModel::GraphicsObject& graphicsObject)
    : m_glProject{glProject}
    , m_graphicsObject{graphicsObject}
    , m_vaoHandle{0}
{
    LOG(INFO) << "Creating VAO and buffers for vertex, normal and index data.";

    glGenVertexArrays(1, &m_vaoHandle);

    glGenBuffers(kBufferSize, m_bufferHandle);

    this->initialize();

    LOG(INFO) << "Connect model's objectChanged signals.";
    m_graphicsObject.graphicsObjectChanged.connect<GlViewObject, &GlViewObject::initialize>(this);
}

void GlViewObject::render() const
{
    float matrixData[16]; // for passing to opengl (both 3x3 & 4x4 matrix)
    auto& shaderProgram = m_glProject.shaderProgram();

    Core::Matrix4x4 modelViewMatrix = m_glProject.gfxProject()->viewpointCamera().viewMatrix()
                                        * m_graphicsObject.geometry()->transformMatrix();
    modelViewMatrix.data(matrixData);
    glUniformMatrix4fv(shaderProgram->modelViewMatrixLocation(), 1, GL_FALSE, matrixData);

    Core::Matrix3x3 normalMatrix = modelViewMatrix;
    normalMatrix.invert();
    normalMatrix.transpose();
    normalMatrix.data(matrixData);
    glUniformMatrix3fv(shaderProgram->normalMatrixLocation(), 1, GL_FALSE, matrixData);

    const Core::Material& material = m_graphicsObject.geometry()->material();

    const Core::Color& diffuseColor = material.diffuseColor();
    glUniform4f(shaderProgram->diffuseColorLocation(), diffuseColor.r(), diffuseColor.g(), diffuseColor.b(), 1.0f);

    const Core::Color& specularColor = material.specularColor();
    glUniform4f(shaderProgram->specularColorLocation(), specularColor.r(), specularColor.g(), specularColor.b(), 1.0f);

    glUniform1f(shaderProgram->shininessLocation(), material.shininess());

    glBindVertexArray(m_vaoHandle);
    const std::vector<int>& indices = m_graphicsObject.indices();
    if (indices.size() > 0)
        glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indices.size()), GL_UNSIGNED_INT, 0);
    else {
        const std::vector<float>& vertices = m_graphicsObject.vertices();
        glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(vertices.size() / GfxModel::GraphicsObject::kVerticesPerTriangle));
    }
}

bool GlViewObject::probe(int x, int y) const
{
    Core::Vector3d nearPoint = this->glNearPoint(x, y);
    Core::Vector3d farPoint = this->glFarPoint(x, y);

    return m_graphicsObject.intersectBoundingBox(nearPoint, farPoint) && m_graphicsObject.intersect(nearPoint, farPoint, nullptr);
}

std::vector<Core::Vector3d> GlViewObject::probePoints(int x, int y) const
{
    std::vector<Core::Vector3d> probePoints{};
    return probePoints;
}

void GlViewObject::initialize()
{
    glBindVertexArray(m_vaoHandle);

    LOG(INFO) << "Binding vertex buffer, filling vertex data and using vertex shader location 0.";

    glBindBuffer(GL_ARRAY_BUFFER, m_bufferHandle[kVertexBuffer]);
    const std::vector<float>& vertices = m_graphicsObject.vertices();
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(kVertexLocation, GfxModel::GraphicsObject::kValuesPerVertex, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(kVertexLocation);

    LOG(INFO) << "Binding normal buffer, filling normal data and using vertex shader location 1.";

    glBindBuffer(GL_ARRAY_BUFFER, m_bufferHandle[kNormalBuffer]);
    const std::vector<float>& normals = m_graphicsObject.normals();
    glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(float), normals.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(kNormalLocation, GfxModel::GraphicsObject::kValuesPerVertex, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(kNormalLocation);

    const std::vector<int>& indices = m_graphicsObject.indices();
    if (indices.size() > 0) {
        LOG(INFO) << "Binding index buffer, filling index data.";

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_bufferHandle[kIndexBuffer]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(int), indices.data(), GL_STATIC_DRAW);
    }

    LOG(INFO) << "Emitting viewChanged signal";
    glViewObjectChanged();
}

Core::Vector3d GlViewObject::glNearPoint(int x, int y) const
{
    Core::Matrix4x4 modelViewMatrix = m_glProject.gfxProject()->viewpointCamera().viewMatrix()
                                        * m_graphicsObject.geometry()->transformMatrix();
    Core::Matrix4x4 projectionMatrix = m_glProject.gfxProject()->projectionMatrix();
    double nearPoint[3];
    gluUnProject(x, y, 0.0, modelViewMatrix.data(), projectionMatrix.data(),
        m_glProject.glViewportTransform().data(), &nearPoint[0], &nearPoint[1], &nearPoint[2]);
    return Core::Vector3d(nearPoint);
}

Core::Vector3d GlViewObject::glFarPoint(int x, int y) const
{
    Core::Matrix4x4 modelViewMatrix = m_glProject.gfxProject()->viewpointCamera().viewMatrix()
                                        * m_graphicsObject.geometry()->transformMatrix();
    Core::Matrix4x4 projectionMatrix = m_glProject.gfxProject()->projectionMatrix();
    double farPoint[3];
    gluUnProject(x, y, 1.0, modelViewMatrix.data(), projectionMatrix.data(),
        m_glProject.glViewportTransform().data(), &farPoint[0], &farPoint[1], &farPoint[2]);
    return Core::Vector3d(farPoint);
}

} // namespace GlViewer
