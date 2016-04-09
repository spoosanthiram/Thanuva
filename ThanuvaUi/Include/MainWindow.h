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

namespace Model { class Project; }

namespace ThanuvaUi {

class OpenGLWidget;
class StartUpPage;

class MainWindow : public QMainWindow
{
public:
    MainWindow();

private slots:
    void create();
    void open();
    bool save();
    bool close();

private: // slots
    void handleProjectDirtyChanged();

private:
    void closeEvent(QCloseEvent* closeEvent) override;

    void setupUi();
    void activate();
    void deactivate();
    bool saveChanges();

    Model::Project* m_project{nullptr};

    StartUpPage* m_startUpPage{nullptr};
    OpenGLWidget* m_openGLWidget{nullptr};

    QAction* m_saveAction{nullptr};
};

} // namespace ThanuvaUi

#endif // THANUVAUI_MAINWINDOW_H
