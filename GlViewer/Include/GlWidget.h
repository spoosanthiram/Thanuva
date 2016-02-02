/**
 * Header file for graphics diplay
 *
 * Author: Saravanan Poosanthiram
 */

#ifndef GLVIEWER_GLWIDGET_H
#define GLVIEWER_GLWIDGET_H

#include <memory>
#include <vector>

#include <QOpenGLWidget>
#include <QMenu>

#include "GlProject.h"
#include "ViewpointCameraModel.h"

namespace Model { class Project; }

namespace GlViewer {

class GlWidget : public QOpenGLWidget {
    Q_OBJECT

public:
    static const int kOpenGLMajorVersion = 4;
    static const int kOpenGLMinorVersion = 3;

    struct Location
    {
        Location() : x{0}, y{0}  {}
        Location(int xval, int yval) : x{xval}, y{yval} {}
        int distance(const Location& rhs) const
        {
            return std::sqrt((rhs.x - x) * (rhs.x - x) + (rhs.y - y) * (rhs.y - y));
        }
        bool operator==(const Location& rhs) { return rhs.x == x && rhs.y == y; }
        int x;
        int y;
    };

public:
    GlWidget(QWidget* parent);

    void activate(Model::Project* project);
    void deactivate();

protected slots:
    void handleViewChanged() { this->update(); }
    void addBox();
    void addStl();

protected:
    void initializeGL() override;
    void resizeGL(int width, int height) override;
    void paintGL() override;

    void mouseMoveEvent(QMouseEvent* mouseEvent) override;
    void mousePressEvent(QMouseEvent* mouseEvent) override;
    void mouseReleaseEvent(QMouseEvent* mouseEvent) override;
    void wheelEvent(QWheelEvent* wheelEvent) override;

    Model::ViewpointCameraModel::Location normalize(const Location& pos)
    {
        return Model::ViewpointCameraModel::Location{(2.0 * pos.x - m_width) / static_cast<double>(m_width),
                    (m_height - 2.0 * pos.y) / static_cast<double>(m_height)};
    }

private:
    GlProject m_glProject;
    Model::Project* m_project;
    int m_width;
    int m_height;
    Location m_currentLocation;
    Location m_previousLocation;

    QMenu* m_contextMenu;
};

} // namespace GlViewer

#endif // GLVIEWER_GLWIDGET_H
