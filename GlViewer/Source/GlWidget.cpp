/**
 * Author: Saravanan Poosanthiram
 * $LastChangedBy: ps $
 * $LastChangedDate: 2015-10-23 21:50:42 -0400 (Fri, 23 Oct 2015) $
 */

#include "GlWidget.h"

#include <memory>

#define GLOG_NO_ABBREVIATED_SEVERITIES
#include "glog/logging.h"

#include <QDir>
#include <QFileDialog>
#include <QMessageBox>
#include <QMouseEvent>
#define Q_ENABLE_OPENGL_FUNCTIONS_DEBUG
#include <QOpenGLContext>
#include <QOpenGLFunctions_3_3_Core>

#include "AppSettings.h"
#include "Box.h"
#include "BoxDialog.h"
#include "Project.h"
#include "Stl.h"

namespace GlViewer {

GlWidget::GlWidget(QWidget* parent)
    : QOpenGLWidget{parent}
    , m_glProject{}
    , m_project{nullptr}
    , m_width{0}
    , m_height{0}
    , m_currentLocation{}
    , m_previousLocation{}
{
    this->setFocusPolicy(Qt::StrongFocus);

    m_contextMenu = new QMenu(this);
    //m_contextMenu->setWindowOpacity(AppSettings().windowOpacity());
    m_contextMenu->addAction(tr("Add Box"), this, SLOT(addBox()));
    m_contextMenu->addAction(tr("Add STL"), this, SLOT(addStl()));

    QSurfaceFormat format = this->format();
    format.setDepthBufferSize(24);
    format.setStencilBufferSize(8);
    format.setVersion(3, 3);
    format.setProfile(QSurfaceFormat::CoreProfile);
    format.setOptions(QSurfaceFormat::StereoBuffers);
    format.setSwapBehavior(QSurfaceFormat::DoubleBuffer);

    //LOG(INFO) << "Setting OpenGL context with version "
    //          << kOpenGLMajorVersion << "." << kOpenGLMinorVersion << " core profile.";
    this->setFormat(format);
}

void GlWidget::activate(Model::Project* project)
{
    LOG(INFO) << "Activating GLWidget for: " << project->name();

    m_project = project;
    m_glProject.activate(m_project);

    connect(&m_glProject, &GlProject::viewChanged, this, &GlWidget::handleViewChanged);
}

void GlWidget::deactivate()
{
    LOG(INFO) << "Deactivating GLWidget for: " << m_project->name();

    disconnect(&m_glProject, 0, this, 0);

    m_glProject.deactivate();
    m_project = nullptr;
}

void GlWidget::addBox()
{
    LOG(INFO) << "Creating default Box, adding it to Model::Project. Showing BoxDialog...";
    auto box = std::make_shared<Model::Box>(*m_project);
    box->setMaterial(Core::Material::defaultMaterial());

    m_project->add(box);

    BoxDialog(this, box).exec();
}

void GlWidget::addStl()
{
    QDir projectDir{m_project->path().c_str()};
    QString filePath = QFileDialog::getOpenFileName(this, "Open File",
                                                    projectDir.absolutePath(), "STL Files (*.stl)");
    if (filePath.isEmpty())
        return;

    LOG(INFO) << "Creating Stl object with chosen file, adding it to Model::Project.";
    try {
        filePath = projectDir.absoluteFilePath(filePath);
        auto stl = std::make_shared<Model::Stl>(*m_project, std::string{filePath.toUtf8().data()});
        stl->setMaterial(Core::Material::defaultMaterial());

        m_project->add(stl);
    }
    catch (const std::exception& e) {
        LOG(WARNING) << e.what();
        QMessageBox::warning(this, qApp->applicationName(), tr(e.what()));
    }
}

void GlWidget::initializeGL()
{
/*
    GLenum err = glewInit();
    if (err != GLEW_OK) {
        std::string msg = std::string{"Could not initialize OpenGL "}
            + std::to_string(kOpenGLMajorVersion) + "." + std::to_string(kOpenGLMinorVersion)
            + std::string{" core profile functions...exiting the program."};
        LOG(ERROR) << msg;
        QMessageBox::critical(this, qApp->applicationName(), QString(msg.c_str()));
        qApp->closeAllWindows();
    }
*/

    QOpenGLFunctions_3_3_Core* glFuncsPtr =
            this->context()->versionFunctions<QOpenGLFunctions_3_3_Core>();
    if (!glFuncsPtr) {
        QMessageBox::critical(this, qApp->applicationName(), "context()->versionFunctions<QOpenGLFunctions_3_3_Core>() failed!");
        qApp->closeAllWindows();
    }
    glFuncsPtr->initializeOpenGLFunctions();
    m_glProject.setGlFuncsPtr(glFuncsPtr);;

    LOG(INFO) << "GL_VENDER: " << glFuncsPtr->glGetString(GL_VENDOR);
    LOG(INFO) << "GL_RENDERER: " << glFuncsPtr->glGetString(GL_RENDERER);
    LOG(INFO) << "GL_VERSION: " << glFuncsPtr->glGetString(GL_VERSION);
    LOG(INFO) << "GL_SHADING_LANGUAGE_VERSION: " << glFuncsPtr->glGetString(GL_SHADING_LANGUAGE_VERSION);

    glFuncsPtr->glEnable(GL_DEPTH_TEST);
    glFuncsPtr->glDepthFunc(GL_LEQUAL);

    glFuncsPtr->glClearColor(0.3f, 0.3f, 0.3f, 1.0f);

    LOG(INFO) << "Loading shaders...";
    try {
        m_glProject.loadShaders();
    }
    catch (const std::exception& e) {
        LOG(ERROR) << e.what();
        QMessageBox::critical(this,
                              qApp->applicationName(),
                              "Loading of GLSL shaders failed...exiting the program.");
        qApp->closeAllWindows();
    }
}

void GlWidget::resizeGL(int width, int height)
{
    if (width == m_width && height == m_height)
        return;

    m_width = width;
    m_height = height;

    LOG(INFO) << "Adjusting Projection and Viewport for window resize.";
    m_glProject.gfxProject()->adjustProjection(m_width, m_height);
    m_glProject.glFuncsPtr()->glViewport(0, 0, m_width, m_height);

    std::array<int, 4> glViewport;
    m_glProject.glFuncsPtr()->glGetIntegerv(GL_VIEWPORT, &glViewport[0]);
    m_glProject.setViewportTransform(glViewport);
}

void GlWidget::paintGL()
{
    m_glProject.glFuncsPtr()->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_glProject.render();
}

void GlWidget::mouseMoveEvent(QMouseEvent* mouseEvent)
{
    m_currentLocation.x = mouseEvent->x();
    m_currentLocation.y = mouseEvent->y();

    if (m_currentLocation.distance(m_previousLocation) < 1)
        return;

    Model::ViewpointCameraModel::Location startLoc = this->normalize(m_previousLocation);
    Model::ViewpointCameraModel::Location endLoc = this->normalize(m_currentLocation);

    m_project->viewpointCameraModel().rotate(startLoc, endLoc);

    m_previousLocation = m_currentLocation;
}

void GlWidget::mousePressEvent(QMouseEvent* mouseEvent)
{
    m_currentLocation.x = mouseEvent->x();
    m_currentLocation.y = mouseEvent->y();

    m_previousLocation = m_currentLocation;
}

void GlWidget::mouseReleaseEvent(QMouseEvent* mouseEvent)
{
    m_currentLocation.x = mouseEvent->x();
    m_currentLocation.y = mouseEvent->y();

    if (mouseEvent->button() == Qt::RightButton) {
        auto probePoints = m_glProject.probe(m_currentLocation.x, m_height - m_currentLocation.y);
        m_contextMenu->exec(mouseEvent->globalPos());
    }

    m_previousLocation = m_currentLocation;
}

void GlWidget::wheelEvent(QWheelEvent* wheelEvent)
{
    m_project->viewpointCameraModel().zoom(wheelEvent->delta() / 120);
}

} // namespace GlViewer
