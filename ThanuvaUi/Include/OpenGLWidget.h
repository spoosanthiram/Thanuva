/*
 * ThanuvaUi: UI for Thanuva
 *
 * Copyright 2016, Saravanan Poosanthiram
 * All rights reserved.
 */

#ifndef ThanuvaUi_OpenGLWidget_h
#define ThanuvaUi_OpenGLWidget_h

#include <memory>
#include <vector>

#include <QMenu>
#include <QOpenGLWidget>

#include "AxisLegendScene.h"
#include "GraphicsScene.h"
#include "ViewpointCamera.h"

namespace Model { class Scene; }

namespace ThanuvaUi {

class MainWindow;

class OpenGLWidget : public QOpenGLWidget
{
    Q_OBJECT

public:
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
    OpenGLWidget(MainWindow* mainWindow);

    void activate(Model::Scene* scene);
    void deactivate();

protected slots:
    void handleViewChanged() { this->update(); }

    void createBox();
    void createCylinder();
    void createCone();
    void createSphere();

    void importMesh();

protected:
    void initializeGL() override;
    void resizeGL(int width, int height) override;
    void paintGL() override;

    void mouseMoveEvent(QMouseEvent* mouseEvent) override;
    void mousePressEvent(QMouseEvent* mouseEvent) override;
    void mouseReleaseEvent(QMouseEvent* mouseEvent) override;
    void wheelEvent(QWheelEvent* wheelEvent) override;

    Graphics::ViewpointCamera::Location normalize(const Location& pos)
    {
        return Graphics::ViewpointCamera::Location{
            (2.0 * pos.x - m_width) / static_cast<double>(m_width),
            (m_height - 2.0 * pos.y) / static_cast<double>(m_height)};
    }

private:
    Model::Scene* m_scene{nullptr};
    Graphics::GraphicsScene m_graphicsScene{};
    Graphics::AxisLegendScene m_axisLegendScene;
    int m_width;
    int m_height;
    Location m_currentLocation;
    Location m_previousLocation;

    QMenu* m_contextMenu;
};

} // namespace ThanuvaUi

#endif // ThanuvaUi_OpenGLWidget_h
