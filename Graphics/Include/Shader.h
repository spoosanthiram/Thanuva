/*
 * Graphics: Graphical objects for Thanuva Model/Geometry obejcts
 *
 * Copyright 2016, Saravanan Poosanthiram
 * All rights reserved.
 */

#ifndef GRAPHICS_SHADER_H
#define GRAPHICS_SHADER_H

#include <string>

#include "OpenGLInterface.h"

namespace Graphics {

class Shader
{
public:
    static const int kLogLen = 1023;

    static std::string shaderTypeStr(GLenum shaderType);

public:
    Shader(GLenum shaderType);
    ~Shader();

    GLenum shaderType() const { return m_shaderType; }
    GLuint handle() const { return m_handle; }

    void compile(const std::string& shaderSource);

protected:
    GLenum m_shaderType;
    GLuint m_handle;
};

} // namespace Graphics

#endif // GRAPHICS_SHADER_H
