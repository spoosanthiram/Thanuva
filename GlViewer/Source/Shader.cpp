/**
* Author: Saravanan Poosanthiram
* $LastChangedBy: ps $
* $LastChangedDate: 2015-03-14 12:57:44 -0400 (Sat, 14 Mar 2015) $
*/

#include "Shader.h"

#include <fstream>

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

Shader::Shader(GLenum shaderType)
    : m_shaderType{shaderType}
    , m_handle{0}
{
    m_handle = glCreateShader(shaderType);
    if (0 == m_handle)
        throw GlViewerException{GlViewerException::kCreateShader + Shader::shaderTypeStr(shaderType)};
}

Shader::~Shader()
{
    glDeleteShader(m_handle);
}

void Shader::compile(const char* shaderSource)
{
    glShaderSource(m_handle, 1, &shaderSource, NULL);

    glCompileShader(m_handle);

    GLint isCompiled;
    glGetShaderiv(m_handle, GL_COMPILE_STATUS, &isCompiled);
    if (!isCompiled)
    {
        GLsizei len;
        glGetShaderiv(m_handle, GL_INFO_LOG_LENGTH, &len);

        char log[kLogLen + 1];
        glGetShaderInfoLog(m_handle, kLogLen, &len, log);

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
