/**
* Author: Saravanan Poosanthiram
* $LastChangedBy: ps $
* $LastChangedDate: 2015-03-20 18:08:01 -0400 (Fri, 20 Mar 2015) $
*/

#ifndef GFXMODEL_GFXPROJECT_H
#define GFXMODEL_GFXPROJECT_H

#include <memory>
#include <vector>

#include "nano_signal_slot.hpp"

#include "GraphicsObject.h"
#include "ViewpointCamera.h"

namespace Model {
    class Geometry;
    class Project;
}

namespace GfxModel {

class GfxProject : public Nano::Observer {
public:
    static const double kDefaultWindowAspect;

public:
    GfxProject(Model::Project* project);
    GfxProject(const GfxProject& other) = delete;
    ~GfxProject();

    GfxProject& operator=(const GfxProject& other) = delete;

    const std::vector<GraphicsObject*>& graphicsObjectList() const { return m_graphicsObjectList; }
    const ViewpointCamera& viewpointCamera() const { return m_viewpointCamera; }
    ViewpointCamera& viewpointCamera() { return m_viewpointCamera; }
    const Extent& extent() const { return m_extent; }
    const Core::Matrix4x4& projectionMatrix() const { return m_projectionMatrix; }

    void adjustProjection(int width, int height);
    void add(Model::Geometry* geometry);

public: // signals
    Nano::Signal<void(GraphicsObject&)> graphicsObjectAdded;
    Nano::Signal<void()> extentChanged;

protected: // slots
    void updateExtent();

private:
    void updateProjectionMatrix();

    Model::Project* m_project;

    std::vector<GraphicsObject*> m_graphicsObjectList;
    ViewpointCamera m_viewpointCamera;
    Extent m_extent;
    double m_windowAspect;
    Core::Matrix4x4 m_projectionMatrix;
};

} // namespace GfxModel

#endif // GFXMODEL_GFXPROJECT_H
