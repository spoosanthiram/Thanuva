/**
* Author: Saravanan Poosanthiram
* $LastChangedBy: ps $
* $LastChangedDate: 2015-03-14 12:57:44 -0400 (Sat, 14 Mar 2015) $
*/

#include "Shader.h"

#include <fstream>

#define Q_ENABLE_OPENGL_FUNCTIONS_DEBUG
#include <QOpenGLFunctions_3_3_Core>

#include "GlProject.h"
#include "GlViewerException.h"

namespace GlViewer {

std::string Shader::shaderTypeStr(GLenum shaderType)
{
    std::string shaderStr = "Unknown Shader";

    if (GL_VERTEX_SHADER == shaderType)
        shaderStr = "Vertex Shader";
    else if (GL_FRAGMENT_SHADER == shaderType)
        shaderStr = "Fragment Shader";

    return shaderStr;
}

Shader::Shader(const GlProject* glProject, GLenum shaderType)
    : m_glProject{glProject}
    , m_shaderType{shaderType}
    , m_handle{0}
{
    m_handle = m_glProject->glFuncsPtr()->glCreateShader(shaderType);
    if (0 == m_handle)
        throw GlViewerException{GlViewerException::kCreateShader + Shader::shaderTypeStr(shaderType)};
}

Shader::~Shader()
{
    //m_glProject->glFuncsPtr()->glDeleteShader(m_handle);
}

void Shader::compile(const char* shaderSource)
{
    m_glProject->glFuncsPtr()->glShaderSource(m_handle, 1, &shaderSource, NULL);

    m_glProject->glFuncsPtr()->glCompileShader(m_handle);

    GLint isCompiled;
    m_glProject->glFuncsPtr()->glGetShaderiv(m_handle, GL_COMPILE_STATUS, &isCompiled);
    if (!isCompiled)
    {
        GLsizei len;
        m_glProject->glFuncsPtr()->glGetShaderiv(m_handle, GL_INFO_LOG_LENGTH, &len);

        char log[kLogLen + 1];
        m_glProject->glFuncsPtr()->glGetShaderInfoLog(m_handle, kLogLen, &len, log);

        throw GlViewerException{std::string{GlViewerException::kCompileShader} + log};
    }
}

/*
std::string Shader::readSource(const std::string& filePath)
{
    std::ifstream shaderStream(filePath, std::ios_base::in);
    if (!shaderStream.is_open())
        throw GlViewerException{GlViewerException::kFileOpen + filePath};

    std::string source;

    std::string str;
    while (!shaderStream.eof())
    {
        std::getline(shaderStream, str);
        source.append(str);
        source.push_back('\n');
    }

    return source;
}
*/

} // namespace GlViewer
