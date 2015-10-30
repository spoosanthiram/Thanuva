/**
 * Author: Saravanan Poosanthiram
 * $LastChangedBy: ps $
 * $LastChangedDate: 2015-03-19 01:09:08 -0400 (Thu, 19 Mar 2015) $
 */

#ifndef GLVIEWER_MAINWINDOW_H
#define GLVIEWER_MAINWINDOW_H

#include <memory>

#include <QMainWindow>

namespace Model { class Project; }

namespace GlViewer {

class GlWidget;

class MainWindow : public QMainWindow/*, public Nano::Observer*/
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

    QAction* m_saveAction = nullptr;
    GlWidget* m_glWidget = nullptr;
};

} // namespace GlViewer

#endif // GLVIEWER_MAINWINDOW_H
