/*
 * Graphics: Graphical objects for Thanuva Model/Geometry obejcts
 *
 * Copyright 2016, Saravanan Poosanthiram
 * All rights reserved.
 */

#ifndef GRAPHICS_SHADERPROGRAM_H
#define GRAPHICS_SHADERPROGRAM_H

#include <memory>
#include <vector>

#include "Shader.h"

namespace Graphics {

class ShaderProgram
{
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
    ShaderProgram();
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
    GLuint m_handle{0};
    std::vector<Shader> m_shaders{};

    GLint m_ambientLightLocation{-1};

    GLint m_light0PositionLocation{-1};
    GLint m_light0DiffuseColorLocation{-1};
    GLint m_light0SpecularColorLocation{-1};

    GLint m_projectionMatrixLocation{-1};
    GLint m_modelViewMatrixLocation{-1};
    GLint m_normalMatrixLocation{-1};

    GLint m_diffuseColorLocation{-1};
    GLint m_specularColorLocation{-1};
    GLint m_shininessLocation{-1};
};

} // namespace Graphics

#endif // GRAPHICS_SHADERPROGRAM_H
