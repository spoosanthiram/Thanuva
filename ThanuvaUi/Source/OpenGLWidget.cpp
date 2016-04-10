/*
 * ThanuvaUi: UI for Thanuva
 *
 * Copyright 2016, Saravanan Poosanthiram
 * All rights reserved.
 */

#include "OpenGLWidget.h"

#include <memory>

#include <glog/logging.h>

#include <QDir>
#include <QFileDialog>
#include <QMessageBox>
#include <QMouseEvent>
#include <QOpenGLContext>

#include "AppSettings.h"
#include "BoxModel.h"
#include "BoxDialog.h"
#include "OpenGLInterface.h"
#include "Project.h"
#include "StlModel.h"

namespace ThanuvaUi {

OpenGLWidget::OpenGLWidget(QWidget* parent)
    : QOpenGLWidget{parent}
    , m_width{0}
    , m_height{0}
    , m_currentLocation{}
    , m_previousLocation{}
{
    this->setFocusPolicy(Qt::StrongFocus);

    m_contextMenu = new QMenu(this);
    m_contextMenu->addAction(tr("Add Box"), this, SLOT(addBox()));
    m_contextMenu->addAction(tr("Add STL"), this, SLOT(addStl()));

    QSurfaceFormat format = this->format();
    format.setVersion(kOpenGLMajorVersion, kOpenGLMinorVersion);
    format.setProfile(QSurfaceFormat::CoreProfile);
    format.setDepthBufferSize(32);
    format.setSwapBehavior(QSurfaceFormat::DoubleBuffer);

    //LOG(INFO) << "Setting OpenGL context with version "
    //          << kOpenGLMajorVersion << "." << kOpenGLMinorVersion << " core profile.";
    this->setFormat(format);
}

void OpenGLWidget::activate(Model::Project* project)
{
    LOG(INFO) << "Activating GLWidget for: " << project->name();

    m_project = project;
    m_graphicsEnvironment.activate(m_project);

    connect(&m_graphicsEnvironment, &Graphics::GraphicsEnvironment::viewChanged,
            this, &OpenGLWidget::handleViewChanged);
}

void OpenGLWidget::deactivate()
{
    LOG(INFO) << "Deactivating GLWidget for: " << m_project->name();

    disconnect(&m_graphicsEnvironment, 0, this, 0);

    m_graphicsEnvironment.deactivate();
    m_project = nullptr;
}

void OpenGLWidget::addBox()
{
    LOG(INFO) << "Creating default Box, adding it to Model::Project. Showing BoxDialog...";
    auto boxModel = std::make_shared<Model::BoxModel>(*m_project);
    boxModel->setMaterial(Core::Material::defaultMaterial());

    this->makeCurrent();

    m_project->add(boxModel);

    BoxDialog(this, boxModel).exec();

    this->doneCurrent();
}

void OpenGLWidget::addStl()
{
    QDir projectDir{m_project->path().c_str()};
    QString filePath = QFileDialog::getOpenFileName(this, "Open File",
                                                    projectDir.absolutePath(), "STL Files (*.stl)");
    if (filePath.isEmpty())
        return;

    LOG(INFO) << "Creating Stl object with chosen file, adding it to Model::Project.";
    try {
        filePath = projectDir.absoluteFilePath(filePath);
        auto stlModel = std::make_shared<Model::StlModel>(*m_project, std::string{filePath.toUtf8().data()});
        stlModel->setMaterial(Core::Material::defaultMaterial());

        this->makeCurrent();
        m_project->add(stlModel);
        this->doneCurrent();
    }
    catch (const std::exception& e) {
        LOG(WARNING) << e.what();
        QMessageBox::warning(this, qApp->applicationName(), tr(e.what()));
    }
}

void OpenGLWidget::initializeGL()
{
    g_OpenGLFuncs = this->context()->versionFunctions<OpenGLFuncs>();
    if (!g_OpenGLFuncs) {
        QMessageBox::critical(this, qApp->applicationName(), "context()->versionFunctions<QOpenGLFunctions_?_?_Core>() failed!");
        qApp->closeAllWindows();
    }
    g_OpenGLFuncs->initializeOpenGLFunctions();

    LOG(INFO) << "GL_VENDER: " << g_OpenGLFuncs->glGetString(GL_VENDOR);
    LOG(INFO) << "GL_RENDERER: " << g_OpenGLFuncs->glGetString(GL_RENDERER);
    LOG(INFO) << "GL_VERSION: " << g_OpenGLFuncs->glGetString(GL_VERSION);
    LOG(INFO) << "GL_SHADING_LANGUAGE_VERSION: " << g_OpenGLFuncs->glGetString(GL_SHADING_LANGUAGE_VERSION);

    g_OpenGLFuncs->glEnable(GL_DEPTH_TEST);
    g_OpenGLFuncs->glDepthFunc(GL_LEQUAL);

    g_OpenGLFuncs->glClearColor(0.3f, 0.3f, 0.3f, 1.0f);

    LOG(INFO) << "Loading shaders...";
    try {
        m_graphicsEnvironment.loadShaders();
    }
    catch (const std::exception& e) {
        LOG(ERROR) << e.what();
        QMessageBox::critical(this,
                              qApp->applicationName(),
                              "Loading of GLSL shaders failed...exiting the program.");
        qApp->closeAllWindows();
    }
}

void OpenGLWidget::resizeGL(int width, int height)
{
    if (width == m_width && height == m_height)
        return;

    m_width = width;
    m_height = height;

    LOG(INFO) << "Adjusting Projection and Viewport for window resize.";
    m_graphicsEnvironment.adjustProjection(m_width, m_height);
    g_OpenGLFuncs->glViewport(0, 0, m_width, m_height);

    std::array<int, 4> glViewport;
    g_OpenGLFuncs->glGetIntegerv(GL_VIEWPORT, &glViewport[0]);
    m_graphicsEnvironment.setViewportTransform(glViewport);
}

void OpenGLWidget::paintGL()
{
    g_OpenGLFuncs->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_graphicsEnvironment.render();
}

void OpenGLWidget::mouseMoveEvent(QMouseEvent* mouseEvent)
{
    m_currentLocation.x = mouseEvent->x();
    m_currentLocation.y = mouseEvent->y();

    if (m_currentLocation.distance(m_previousLocation) < 1)
        return;

    Graphics::ViewpointCamera::Location startLoc = this->normalize(m_previousLocation);
    Graphics::ViewpointCamera::Location endLoc = this->normalize(m_currentLocation);

    m_graphicsEnvironment.viewpointCamera().rotate(startLoc, endLoc);

    m_previousLocation = m_currentLocation;
}

void OpenGLWidget::mousePressEvent(QMouseEvent* mouseEvent)
{
    m_currentLocation.x = mouseEvent->x();
    m_currentLocation.y = mouseEvent->y();

    m_previousLocation = m_currentLocation;
}

void OpenGLWidget::mouseReleaseEvent(QMouseEvent* mouseEvent)
{
    m_currentLocation.x = mouseEvent->x();
    m_currentLocation.y = mouseEvent->y();

    if (mouseEvent->button() == Qt::RightButton) {
        auto probePoints = m_graphicsEnvironment.probe(m_currentLocation.x, m_height - m_currentLocation.y);
        m_contextMenu->exec(mouseEvent->globalPos());
    }

    m_previousLocation = m_currentLocation;
}

void OpenGLWidget::wheelEvent(QWheelEvent* wheelEvent)
{
    m_graphicsEnvironment.viewpointCamera().zoom(wheelEvent->delta() / 120);
}

} // namespace ThanuvaUi
