/**
* Author: Saravanan Poosanthiram
* $LastChangedBy: ps $
* $LastChangedDate: 2015-03-14 12:57:44 -0400 (Sat, 14 Mar 2015) $
*/

#include "ShaderProgram.h"

#include <QOpenGLFunctions_4_3_Core>

#include "GlProject.h"
#include "GlViewerException.h"

namespace GlViewer {

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

ShaderProgram::ShaderProgram(const GlProject* glProject)
    : m_glProject{glProject}
    , m_handle{0}
    , m_shaders{}
    , m_ambientLightLocation{-1}
    , m_light0PositionLocation{-1}
    , m_light0DiffuseColorLocation{-1}
    , m_light0SpecularColorLocation{-1}
    , m_projectionMatrixLocation{-1}
    , m_modelViewMatrixLocation{-1}
    , m_normalMatrixLocation{-1}
    , m_diffuseColorLocation{-1}
    , m_specularColorLocation{-1}
    , m_shininessLocation{-1}
{
    m_handle = m_glProject->glFuncsPtr()->glCreateProgram();
    if (0 == m_handle)
        throw GlViewerException{GlViewerException::kCreateProgram};
}

ShaderProgram::~ShaderProgram()
{
    for (auto shader : m_shaders)
        m_glProject->glFuncsPtr()->glDetachShader(m_handle, shader.handle());

    m_glProject->glFuncsPtr()->glDeleteProgram(m_handle);
}

void ShaderProgram::attachShader(const Shader& shader)
{
    m_glProject->glFuncsPtr()->glAttachShader(m_handle, shader.handle());
    if (m_glProject->glFuncsPtr()->glGetError() != GL_NO_ERROR)
        throw GlViewerException{GlViewerException::kAttachShader};
    m_shaders.push_back(shader);
}

void ShaderProgram::link()
{
    if (m_shaders.size() <= 0)
        return;

    m_glProject->glFuncsPtr()->glLinkProgram(m_handle);

    GLint isLinked;
    m_glProject->glFuncsPtr()->glGetProgramiv(m_handle, GL_LINK_STATUS, &isLinked);
    if (!isLinked)
    {
        GLsizei len;
        m_glProject->glFuncsPtr()->glGetProgramiv(m_handle, GL_INFO_LOG_LENGTH, &len);

        char log[kLogBufLen + 1];
        m_glProject->glFuncsPtr()->glGetProgramInfoLog(m_handle, kLogBufLen, &len, log);

        throw GlViewerException{std::string{GlViewerException::kLinkProgram} + log};
    }

    this->updateLocations();
}

void ShaderProgram::updateLocations()
{
    m_projectionMatrixLocation = m_glProject->glFuncsPtr()->glGetUniformLocation(m_handle, kProjectionMatrixUniformName);
    m_modelViewMatrixLocation = m_glProject->glFuncsPtr()->glGetUniformLocation(m_handle, kModelViewMatrixUniformName);
    m_normalMatrixLocation = m_glProject->glFuncsPtr()->glGetUniformLocation(m_handle, kNormalMatrixUniformName);

    m_ambientLightLocation = m_glProject->glFuncsPtr()->glGetUniformLocation(m_handle, kAmbientLightUniformName);

    m_light0PositionLocation = m_glProject->glFuncsPtr()->glGetUniformLocation(m_handle, kLight0PositionUniformName);
    m_light0DiffuseColorLocation = m_glProject->glFuncsPtr()->glGetUniformLocation(m_handle, kLight0DiffuseColorUniformName);
    m_light0SpecularColorLocation = m_glProject->glFuncsPtr()->glGetUniformLocation(m_handle, kLight0SpecularColorUniformName);

    m_diffuseColorLocation = m_glProject->glFuncsPtr()->glGetUniformLocation(m_handle, kDiffuseColorUniformName);
    m_specularColorLocation = m_glProject->glFuncsPtr()->glGetUniformLocation(m_handle, kSpecularColorUniformName);
    m_shininessLocation = m_glProject->glFuncsPtr()->glGetUniformLocation(m_handle, kShininessUniformName);
}

} // namespace GlViewer
