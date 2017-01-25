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
#include "ModelException.h"
#include "OpenGLWidget.h"
#include "StartUpPage.h"
#include "ThanuvaApp.h"

namespace ThanuvaUi {

const char* MainWindow::kThanuvaFilesFilter = "Thanuva Files (*{})";

MainWindow::MainWindow(Model::ThanuvaApp& app)
    : QMainWindow{nullptr}
    , m_app{app}
{
    this->setupUi();

    m_startUpPage = new StartUpPage{this};
    this->setCentralWidget(m_startUpPage);

    connect(m_startUpPage, &StartUpPage::newThanuvaProject, this, &MainWindow::create);
}

void MainWindow::create()
{
    if (m_scene && !this->close()) // there is an existing scene, close it!
        return;

    LOG(INFO) << "Creating new scene!";

    m_scene = m_app.newScene();
    this->activate();
    m_saveAsAction->setEnabled(m_scene != nullptr);
}

void MainWindow::open()
{
    if (m_scene && !this->close())
        return;

    QString caption{"Open Scene"};
    QString filePath = QFileDialog::getOpenFileName(this, caption, m_app.recentDirPath().string().c_str(),
                            fmt::format(kThanuvaFilesFilter, m_app.fileExtension().string()).c_str());
    if (filePath.isEmpty())
        return;

    try {
        m_scene = m_app.openScene(filePath.toStdString());
    }
    catch (const Model::ModelException& e) {
        QMessageBox::critical(this, caption, QString{"Could not open the Scene...%1"}.arg(e.what()));
    }

    this->activate();
}

void MainWindow::openRecentScene(QAction* selectedAction)
{
    QString caption{"Recent Scenes"};
    fs::path filePath{selectedAction->text().toStdString()};
    if (!fs::exists(filePath)) {
        QMessageBox::StandardButton answer = QMessageBox::question(this, caption,
            QString{"Select scene '%1' does not exist. Do you want to remove from recent scenes list?"}.arg(selectedAction->text()),
            QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
        if (answer == QMessageBox::Yes)
            m_app.removeFromRecentPaths(filePath);
        return;
    }

    if (m_scene && !this->close())
        return;

    try {
        m_scene = m_app.openScene(filePath);
    }
    catch (const Model::ModelException& e) {
        QMessageBox::critical(this, caption, QString{"Could not open the Scene...%1"}.arg(e.what()));
    }

    this->activate();
}

bool MainWindow::save()
{
    CHECK(m_scene->isSceneChanged());

    if (!m_scene->isNamed())
        return this->saveAs();
    else {
        try {
            m_scene->write();
        }
        catch (const std::exception& e) {
            QString caption{"Save Scene"};
            QMessageBox::critical(this, caption, QString{"Could not save the Scene...%1"}.arg(e.what()));
            return false;
        }
    }

    return true;
}

bool MainWindow::saveAs()
{
    QString caption{"Save Scene As"};
    QString filePath = QFileDialog::getSaveFileName(this, caption, m_app.recentDirPath().string().c_str(),
            fmt::format(kThanuvaFilesFilter, m_app.fileExtension().string()).c_str());
    if (filePath.isEmpty())
        return false;

    if (!filePath.contains('.'))
        filePath.append(m_app.fileExtension().string().c_str());

    try {
        m_app.saveSceneAs(filePath.toStdString());
    }
    catch (const std::exception& e) {
        QMessageBox::critical(this, caption, QString{"Could not save the Scene...%1"}.arg(e.what()));
        return false;
    }

    return true;
}

bool MainWindow::close()
{
    if (!this->saveChanges())
        return false;

    this->deactivate();
    m_app.closeScene();

    m_saveAction->setEnabled(false);
    m_saveAsAction->setEnabled(false);

    return true;
}

void MainWindow::buildRecentScenes()
{
    m_recentScenesAction->clear();

    const std::list<fs::path>& recentPaths = this->app().recentScenePaths();
    for (auto& filePath : recentPaths) {
        QAction* action = m_recentScenesAction->addAction(filePath.string().c_str());
    }
}

void MainWindow::handleSceneChanged()
{
    m_saveAction->setEnabled(m_scene->isSceneChanged());

    std::string title = fmt::format("{}{} - {}", m_scene->name(),
                                    m_scene->isSceneChanged() ? "*" : "", m_app.name());
    this->setWindowTitle(title.c_str());
}

void MainWindow::closeEvent(QCloseEvent* closeEvent)
{
    if (m_scene && !this->saveChanges()) {
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

    this->buildFileMenu();
}

void MainWindow::buildFileMenu()
{
    m_fileMenu = this->menuBar()->addMenu(MainWindow::tr("&File"));

    QAction* newAction = m_fileMenu->addAction(MainWindow::tr("&New Scene"));
    connect(newAction, &QAction::triggered, this, &MainWindow::create);

    QAction* openAction = m_fileMenu->addAction(MainWindow::tr("&Open Scene..."));
    connect(openAction, &QAction::triggered, this, &MainWindow::open);

    m_recentScenesAction = m_fileMenu->addMenu(MainWindow::tr("Recent Scenes"));
    connect(m_recentScenesAction, &QMenu::triggered, this, &MainWindow::openRecentScene);

    m_fileMenu->addSeparator();

    m_saveAction = m_fileMenu->addAction(MainWindow::tr("Save"));
    m_saveAction->setEnabled(false);
    connect(m_saveAction, &QAction::triggered, this, &MainWindow::save);

    m_saveAsAction = m_fileMenu->addAction(MainWindow::tr("Save &As..."));
    m_saveAsAction->setEnabled(false);
    connect(m_saveAsAction, &QAction::triggered, this, &MainWindow::saveAs);

    m_fileMenu->addSeparator();

    QAction* exitAction = m_fileMenu->addAction(MainWindow::tr("Exit"));
    connect(exitAction, &QAction::triggered, qApp, &QApplication::closeAllWindows);

    connect(m_fileMenu, &QMenu::aboutToShow, this, &MainWindow::buildRecentScenes);
}

void MainWindow::activate()
{
    CHECK(m_scene) << "MainWindow::activate: nullptr!";

    if (!m_openGLWidget) {
        m_openGLWidget = new OpenGLWidget{this};
        this->setCentralWidget(m_openGLWidget);
        m_openGLWidget->show();
    }

    LOG(INFO) << "Activing scene: " << m_scene->name();

    m_openGLWidget->activate(m_scene);
    this->handleSceneChanged();

    m_scene->sceneChanged.connect<MainWindow, &MainWindow::handleSceneChanged>(this);
}

void MainWindow::deactivate()
{
    CHECK(m_scene) << "Deactivate: nullptr!";
    LOG(INFO) << "Deactivating scene: " << m_scene->name();

    m_openGLWidget->deactivate();

    m_scene->sceneChanged.disconnect<MainWindow, &MainWindow::handleSceneChanged>(this);
}

bool MainWindow::saveChanges()
{
    if (!m_scene->isSceneChanged())
        return true;

    bool retval = true;

    QString caption{"Save Changes"};
    QString message = QString{"The Scene \"%1\" has unsaved changes, do you want to save?"}.arg(
                QString(m_scene->name().c_str()));
    QMessageBox::StandardButton answer = QMessageBox::question(this, caption, message,
                QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel, QMessageBox::Cancel);
    if (QMessageBox::Cancel == answer)
        retval = false;
    else if (QMessageBox::Save == answer)
        retval = this->save();

    return retval;
}

} // namespace ThanuvaUi
