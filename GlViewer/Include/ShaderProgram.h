/**
* Author: Saravanan Poosanthiram
* $LastChangedBy: ps $
* $LastChangedDate: 2015-03-14 12:57:44 -0400 (Sat, 14 Mar 2015) $
*/

#ifndef GLVIEWER_SHADERPROGRAM_H
#define GLVIEWER_SHADERPROGRAM_H

#include <memory>
#include <vector>

#include "Shader.h"

namespace GlViewer {

class GlProject;

class ShaderProgram {
public:
    static const int kLogBufLen = 1000;

    static const char* kAmbientLightUniformName;

    static const char* kLight0PositionUniformName;
    static const char* kLight0DiffuseColorUniformName;
    static const char* kLight0SpecularColorUniformName;

    static const char* kProjectionMatrixUniformName;
    static const char* kModelViewMatrixUniformName;
    static const char* kNormalMatrixUniformName;

    static const char* kDiffuseColorUniformName;
    static const char* kSpecularColorUniformName;
    static const char* kShininessUniformName;

public:
    ShaderProgram(const GlProject* glProject);
    ~ShaderProgram();

    GLuint handle() const { return m_handle; }

    GLint ambientLightLocation() const { return m_ambientLightLocation; }

    GLint light0PositionLocation() const { return m_light0PositionLocation; }
    GLint light0DiffuseColorLocation() const { return m_light0DiffuseColorLocation; }
    GLint light0SpecularColorLocation() const { return m_light0SpecularColorLocation; }

    GLint projectionMatrixLocation() const { return m_projectionMatrixLocation; }
    GLint modelViewMatrixLocation() const { return m_modelViewMatrixLocation; }
    GLint normalMatrixLocation() const { return m_normalMatrixLocation; }

    GLint diffuseColorLocation() const { return m_diffuseColorLocation; }
    GLint specularColorLocation() const { return m_specularColorLocation; }
    GLint shininessLocation() const { return m_shininessLocation; }

    void attachShader(const Shader& shader);
    void link();

protected:
    void updateLocations();

protected:
    const GlProject* m_glProject;

    GLuint m_handle;
    std::vector<Shader> m_shaders;

    GLint m_ambientLightLocation;

    GLint m_light0PositionLocation;
    GLint m_light0DiffuseColorLocation;
    GLint m_light0SpecularColorLocation;

    GLint m_projectionMatrixLocation;
    GLint m_modelViewMatrixLocation;
    GLint m_normalMatrixLocation;

    GLint m_diffuseColorLocation;
    GLint m_specularColorLocation;
    GLint m_shininessLocation;
};

} // namespace GlViewer

#endif // GLVIEWER_SHADERPROGRAM_H
