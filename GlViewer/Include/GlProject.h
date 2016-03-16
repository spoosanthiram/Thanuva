/**
 * Header file Viewer Project class.
 *
 * Author: Saravanan Poosanthiram
 */

#ifndef GLVIEWER_GLPROJECT_H
#define GLVIEWER_GLPROJECT_H

#include <memory>
#include <vector>

#include <QObject>

#include "GfxProject.h"
#include "GlViewObject.h"
#include "ShaderProgram.h"

class QOpenGLFunctions_4_3_Core;
namespace GfxModel { class GeometryObject; }

namespace GlViewer {

class GlProject : public QObject {
    Q_OBJECT

public:
    static const int kNTransformMatrixElements = 16;

public:
    GlProject();

    const std::unique_ptr<GfxModel::GfxProject>& gfxProject() const { return m_gfxProject; }
    QOpenGLFunctions_4_3_Core* glFuncsPtr() const { return m_glFuncsPtr; }
    const std::unique_ptr<ShaderProgram>& shaderProgram() const { return m_shaderProgram; }
    const std::array<int, 4>& glViewportTransform() const { return m_glViewportTransform; }

    void setGlFuncsPtr(QOpenGLFunctions_4_3_Core* funcsPtr) { m_glFuncsPtr = funcsPtr; }
    void setViewportTransform(const std::array<int, 4>& values)
    {
        std::copy(values.cbegin(), values.cend(), m_glViewportTransform.begin());
    }

    void activate(Model::Project* project);
    void deactivate();
    void loadShaders();
    void render() const;
    std::vector<const GlViewer::GlViewObject*> probe(int x, int y) const;

signals:
    void viewChanged();

protected: // slots
    void add(GfxModel::GeometryObject& graphicsObject);
    void handleExtentChanged();
    void emitViewChanged() { emit viewChanged(); } // needed for connecting Nano signal to Qt signal

private:
    std::unique_ptr<GfxModel::GfxProject> m_gfxProject;
    std::vector<GlViewObject*> m_glViewObjectList;

    QOpenGLFunctions_4_3_Core* m_glFuncsPtr;
    std::unique_ptr<ShaderProgram> m_shaderProgram;

    float m_ambientLight[4];

    float m_light0Position[4];
    float m_light0DiffuseColor[4];
    float m_light0SpecularColor[4];

    std::array<int, 4> m_glViewportTransform;

    static const char* kVertexShaderSource;
    static const char* kFragmentShaderSource;
};

} // namespace GlViewer

#endif // GLVIEWER_GLPROJECT_H
