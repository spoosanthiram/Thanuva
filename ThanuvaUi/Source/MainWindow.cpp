/*
 * ThanuvaUi: UI for Thanuva
 *
 * Copyright 2016, Saravanan Poosanthiram
 * All rights reserved.
 */

#include "MainWindow.h"

#include <memory>

#include <fmt/format.h>
#include <glog/logging.h>
#include <nano_signal_slot.hpp>

#include <QApplication>
#include <QCloseEvent>
#include <QFileDialog>
#include <QMessageBox>
#include <QMenuBar>
#include <QStatusBar>
#include <QSurfaceFormat>

#include "AppSettings.h"
#include "OpenGLWidget.h"
#include "Project.h"
#include "StartUpPage.h"

namespace ThanuvaUi {

MainWindow::MainWindow()
    : QMainWindow{nullptr}
{
    this->setupUi();

    m_startUpPage = new StartUpPage{this};
    this->setCentralWidget(m_startUpPage);

    connect(m_startUpPage, &StartUpPage::newThanuvaProject, this, &MainWindow::create);
}

void MainWindow::create()
{
    LOG(INFO) << "Creating new project!";
    m_project = new Model::Project{};

    this->activate();
}

void MainWindow::open()
{
    if (m_project && !this->close())
        return;

    CHECK(nullptr == m_project);

    QDir projectDir{};
    QString filePath = QFileDialog::getOpenFileName(this, "Open Project", projectDir.absolutePath(),
                            fmt::format("Project Files (*.{})", Model::Project::kFileExtention).c_str());
    if (filePath.isEmpty())
        return;

    try {
        m_project = new Model::Project{filePath.toUtf8().data()};
    }
    catch (std::exception& e) {
        QMessageBox::critical(this, "Project Open Error",
                              QString("Not able to open %1. Error: %2").arg(filePath, e.what()));
    }

    this->activate();
}

bool MainWindow::save()
{
    CHECK(m_project->isDirty());

    if (!m_project->isNamed()) {
        QDir projectDir{m_project->path().c_str()};
        QString filePath = QFileDialog::getSaveFileName(this, "Save Project", projectDir.absolutePath(),
                                fmt::format("Project Files (*.{})", Model::Project::kFileExtention).c_str());
        if (filePath.isEmpty())
            return false;

        filePath = projectDir.absoluteFilePath(filePath);

        if (!filePath.contains('.')) {
            filePath.append('.');
            filePath.append(Model::Project::kFileExtention);
        }

        try {
            m_project->setFilePath(filePath.toUtf8().data());
        }
        catch (std::exception& e) {
            QMessageBox::warning(this, "Project Save Error",
                                 QString("Project is not saved! Reason: %1").arg(e.what()));
            return false;
        }
    }

    m_project->save();

    return true;
}

bool MainWindow::close()
{
    if (!this->saveChanges())
        return false;

    this->deactivate();

    delete m_project;
    m_project = nullptr;

    return true;
}

void MainWindow::handleProjectDirtyChanged()
{
    QString title = QString(m_project->name().c_str());
    if (m_project->isDirty()) {
        m_saveAction->setEnabled(true);
        title.append('*');
    }
    else
        m_saveAction->setEnabled(false);
    this->setWindowTitle(title);
}

void MainWindow::closeEvent(QCloseEvent* closeEvent)
{
    if (m_project && !this->saveChanges()) {
        closeEvent->ignore();
        return;
    }

    LOG(INFO) << "Saving settings while closing MainWindow.";

    AppSettings settings;

    if (!this->isMaximized())
        settings.setGeometry(this->geometry());

    settings.setIsMaximized(this->isMaximized());

    closeEvent->accept();
}

void MainWindow::setupUi()
{
    AppSettings settings;
    this->setGeometry(settings.geometry());
    if (settings.isMaximized())
        this->setWindowState(this->windowState() | Qt::WindowMaximized);

    QMenu* fileMenu = this->menuBar()->addMenu(MainWindow::tr("&File"));

    QAction* openAction = fileMenu->addAction(MainWindow::tr("Open..."));
    connect(openAction, &QAction::triggered, this, &MainWindow::open);

    m_saveAction = fileMenu->addAction(MainWindow::tr("Save"));
    connect(m_saveAction, &QAction::triggered, this, &MainWindow::save);

    QAction* exitAction = fileMenu->addAction(MainWindow::tr("Exit"));
    connect(exitAction, &QAction::triggered, qApp, &QApplication::closeAllWindows);
}

void MainWindow::activate()
{
    CHECK(m_project) << "Activate: nullptr!";

    if (!m_openGLWidget) {
        m_openGLWidget = new OpenGLWidget{this};
        this->setCentralWidget(m_openGLWidget);
        m_openGLWidget->show();
    }

    LOG(INFO) << "Activing project: " << m_project->name();

    m_openGLWidget->activate(m_project);
    this->handleProjectDirtyChanged();

    m_project->dirtyChanged.connect<MainWindow, &MainWindow::handleProjectDirtyChanged>(this);
}

void MainWindow::deactivate()
{
    CHECK(m_project) << "Deactivate: nullptr!";

    LOG(INFO) << "Deactivating project: " << m_project->name();

    m_openGLWidget->deactivate();

    m_project->dirtyChanged.disconnect<MainWindow, &MainWindow::handleProjectDirtyChanged>(this);
}

bool MainWindow::saveChanges()
{
    if (!m_project->isDirty())
        return true;

    bool retval = true;

    QString message = QString("Project \"%1\" has unsaved changes, do you want to save?").arg(
                QString(m_project->name().c_str()));
    QMessageBox::StandardButton answer = QMessageBox::question(this, "Save Changes", message,
                QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel, QMessageBox::Cancel);
    if (QMessageBox::Cancel == answer)
        retval = false;
    else if (QMessageBox::Save == answer)
        retval = this->save();

    return retval;
}

} // namespace ThanuvaUi
