/**
* Author: Saravanan Poosanthiram
* $LastChangedBy: ps $
* $LastChangedDate: 2015-03-14 12:57:44 -0400 (Sat, 14 Mar 2015) $
*/

#ifndef GLVIEWER_SHADER_H
#define GLVIEWER_SHADER_H

#include <string>

#include <QOpenGLFunctions>

namespace GlViewer {

class GlProject;

class Shader {
public:
    static const int kLogLen = 1000;

    static std::string shaderTypeStr(GLenum shaderType);

public:
    Shader(const GlProject* glProject, GLenum shaderType);
    ~Shader();

    GLenum shaderType() const { return m_shaderType; }
    GLuint handle() const { return m_handle; }

    void compile(const char* shaderSource);

protected:
    //std::string readSource(const std::string& filePath);

protected:
    const GlProject* m_glProject;

    GLenum m_shaderType;
    GLuint m_handle;
};

} // namespace GlViewer

#endif // GLVIEWER_SHADER_H
