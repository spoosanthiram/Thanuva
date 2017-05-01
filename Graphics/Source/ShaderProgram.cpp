/*
 * Graphics: Graphical objects for Thanuva Model/Geometry obejcts
 *
 * Copyright 2016, Saravanan Poosanthiram
 * All rights reserved.
 */

#include "ShaderProgram.h"

#include "GraphicsException.h"

namespace Graphics {

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
}

} // namespace Graphics
