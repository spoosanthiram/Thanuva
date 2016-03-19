/*
 * ThanuvaUi: UI for Thanuva
 *
 * Copyright 2016, Saravanan Poosanthiram
 * All rights reserved.
 */

#ifndef GLVIEWER_MAINWINDOW_H
#define GLVIEWER_MAINWINDOW_H

#include <memory>

#include <QMainWindow>

namespace Model { class Project; }

namespace GlViewer {

class GlWidget;
class StartUpPage;

class MainWindow : public QMainWindow
{
public:
    MainWindow(QWidget* parent = nullptr);

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

    Model::Project* m_project = nullptr;
    std::string m_lastFileOpLocation{};

    StartUpPage* m_startUpPage = nullptr;
    GlWidget* m_glWidget = nullptr;

    QAction* m_saveAction = nullptr;
};

} // namespace GlViewer

#endif // GLVIEWER_MAINWINDOW_H
