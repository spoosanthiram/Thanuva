/*
 * ThanuvaUi: UI for Thanuva
 *
 * Copyright 2016, Saravanan Poosanthiram
 * All rights reserved.
 */

#ifndef THANUVAUI_MAINWINDOW_H
#define THANUVAUI_MAINWINDOW_H

#include <memory>

#include <QMainWindow>

#include "ThanuvaApp.h"

namespace ThanuvaUi {

class OpenGLWidget;
class StartUpPage;


class MainWindow : public QMainWindow
{
public:
    static const char* kThanuvaFilesFilter;

public:
    MainWindow(Model::ThanuvaApp& app);

    const Model::ThanuvaApp& app() const { return m_app; }

private slots:
    void create();
    void open();
    void openRecentScene(QAction* selectedAction);
    bool save();
    bool saveAs();
    bool close();
    void buildRecentScenes();

private: // slots
    void handleSceneChanged();

private:
    void closeEvent(QCloseEvent* closeEvent) override;

    void setupUi();
    void buildFileMenu();
    void activate();
    void deactivate();
    bool saveChanges();

    Model::ThanuvaApp& m_app;
    Model::Scene* m_scene{nullptr};

    StartUpPage* m_startUpPage{nullptr};
    OpenGLWidget* m_openGLWidget{nullptr};

    QMenu* m_fileMenu{nullptr};
    QMenu* m_recentScenesAction{nullptr};
    QAction* m_saveAction{nullptr};
    QAction* m_saveAsAction{nullptr};
};

} // namespace ThanuvaUi

#endif // THANUVAUI_MAINWINDOW_H
