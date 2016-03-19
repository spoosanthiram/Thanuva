/*
 * Graphics: Graphical objects for Thanuva Model/Geometry obejcts
 *
 * Copyright 2016, Saravanan Poosanthiram
 * All rights reserved.
 */

#include "ShaderProgram.h"

#include "GraphicsException.h"

namespace Graphics {

const char* ShaderProgram::kAmbientLightUniformName = "ambientLight";

const char* ShaderProgram::kLight0PositionUniformName = "light0Position";
const char* ShaderProgram::kLight0DiffuseColorUniformName = "light0DiffuseColor";
const char* ShaderProgram::kLight0SpecularColorUniformName = "light0SpecularColor";

const char* ShaderProgram::kProjectionMatrixUniformName = "projectionMatrix";
const char* ShaderProgram::kModelViewMatrixUniformName = "modelViewMatrix";
const char* ShaderProgram::kNormalMatrixUniformName = "normalMatrix";

const char* ShaderProgram::kDiffuseColorUniformName = "diffuseColor";
const char* ShaderProgram::kSpecularColorUniformName = "specularColor";
const char* ShaderProgram::kShininessUniformName = "shininess";

ShaderProgram::ShaderProgram()
{
    m_handle = g_OpenGLFuncs->glCreateProgram();
    if (0 == m_handle)
        throw GraphicsException{GraphicsException::kCreateProgram};
}

ShaderProgram::~ShaderProgram()
{
    for (auto shader : m_shaders)
        g_OpenGLFuncs->glDetachShader(m_handle, shader.handle());

    g_OpenGLFuncs->glDeleteProgram(m_handle);
}

void ShaderProgram::attachShader(const Shader& shader)
{
    g_OpenGLFuncs->glAttachShader(m_handle, shader.handle());
    if (g_OpenGLFuncs->glGetError() != GL_NO_ERROR)
        throw GraphicsException{GraphicsException::kAttachShader};
    m_shaders.push_back(shader);
}

void ShaderProgram::link()
{
    if (m_shaders.size() <= 0)
        return;

    g_OpenGLFuncs->glLinkProgram(m_handle);

    GLint isLinked;
    g_OpenGLFuncs->glGetProgramiv(m_handle, GL_LINK_STATUS, &isLinked);
    if (!isLinked)
    {
        GLsizei len;
        g_OpenGLFuncs->glGetProgramiv(m_handle, GL_INFO_LOG_LENGTH, &len);

        char log[kLogBufLen + 1];
        g_OpenGLFuncs->glGetProgramInfoLog(m_handle, kLogBufLen, &len, log);

        throw GraphicsException{std::string{GraphicsException::kLinkProgram} + log};
    }

    this->updateLocations();
}

void ShaderProgram::updateLocations()
{
    m_projectionMatrixLocation = g_OpenGLFuncs->glGetUniformLocation(
                                    m_handle, kProjectionMatrixUniformName);
    m_modelViewMatrixLocation = g_OpenGLFuncs->glGetUniformLocation(
                                    m_handle, kModelViewMatrixUniformName);
    m_normalMatrixLocation = g_OpenGLFuncs->glGetUniformLocation(
                                 m_handle, kNormalMatrixUniformName);

    m_ambientLightLocation = g_OpenGLFuncs->glGetUniformLocation(
                                 m_handle, kAmbientLightUniformName);

    m_light0PositionLocation = g_OpenGLFuncs->glGetUniformLocation(
                                   m_handle, kLight0PositionUniformName);
    m_light0DiffuseColorLocation = g_OpenGLFuncs->glGetUniformLocation(
                                       m_handle, kLight0DiffuseColorUniformName);
    m_light0SpecularColorLocation = g_OpenGLFuncs->glGetUniformLocation(
                                        m_handle, kLight0SpecularColorUniformName);

    m_diffuseColorLocation = g_OpenGLFuncs->glGetUniformLocation(m_handle,
                                                                 kDiffuseColorUniformName);
    m_specularColorLocation = g_OpenGLFuncs->glGetUniformLocation(m_handle,
                                                                  kSpecularColorUniformName);
    m_shininessLocation = g_OpenGLFuncs->glGetUniformLocation(m_handle, kShininessUniformName);
}

} // namespace Graphics
