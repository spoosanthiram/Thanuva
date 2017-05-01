/*
 * Graphics: Graphical objects for Thanuva Model/Geometry obejcts
 *
 * Copyright 2016, Saravanan Poosanthiram
 * All rights reserved.
 */

#include "Shader.h"

#include "GraphicsException.h"

namespace Graphics {

std::string Shader::shaderTypeStr(GLenum shaderType)
{
    std::string shaderStr = "Unknown Shader";

    if (GL_VERTEX_SHADER == shaderType)
        shaderStr = "Vertex Shader";
    else if (GL_FRAGMENT_SHADER == shaderType)
        shaderStr = "Fragment Shader";

    return shaderStr;
}

Shader::Shader(GLenum shaderType)
    : m_shaderType{shaderType}
    , m_handle{0}
{
    m_handle = g_OpenGLFuncs->glCreateShader(shaderType);
    if (0 == m_handle)
        throw GraphicsException{GraphicsException::kCreateShader + Shader::shaderTypeStr(shaderType)};
}

Shader::~Shader()
{
    //g_OpenGLFuncs->glDeleteShader(m_handle);
}

void Shader::compile(const std::string& shaderSource)
{
    const char* source = shaderSource.c_str();
    g_OpenGLFuncs->glShaderSource(m_handle, 1, &source, NULL);

    g_OpenGLFuncs->glCompileShader(m_handle);

    GLint isCompiled;
    g_OpenGLFuncs->glGetShaderiv(m_handle, GL_COMPILE_STATUS, &isCompiled);
    if (!isCompiled)
    {
        GLsizei len;
        g_OpenGLFuncs->glGetShaderiv(m_handle, GL_INFO_LOG_LENGTH, &len);

        char log[kLogLen + 1];
        g_OpenGLFuncs->glGetShaderInfoLog(m_handle, kLogLen, &len, log);

        throw GraphicsException{std::string{GraphicsException::kCompileShader} + log};
    }
}

} // namespace Graphics
