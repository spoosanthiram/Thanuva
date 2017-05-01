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

public:
    ShaderProgram();
    ~ShaderProgram();

    GLuint handle() const { return m_handle; }

    void attachShader(const Shader& shader);
    void link();

protected:
    GLuint m_handle{0};
    std::vector<Shader> m_shaders{};
};

} // namespace Graphics

#endif // GRAPHICS_SHADERPROGRAM_H
