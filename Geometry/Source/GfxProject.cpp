/**
* Author: Saravanan Poosanthiram
* $LastChangedBy: ps $
* $LastChangedDate: 2015-03-20 18:08:01 -0400 (Fri, 20 Mar 2015) $
*/

#include "GfxProject.h"

#include "Box.h"
#include "GfxBox.h"
#include "GfxStl.h"
#include "Project.h"
#include "Stl.h"

#include "glog/logging.h"

namespace GfxModel {

const double GfxProject::kDefaultWindowAspect = 1.6;

GfxProject::GfxProject(Model::Project* project)
    : graphicsObjectAdded{}
    , m_project{project}
    , m_graphicsObjectList{}
    , m_viewpointCamera{*this, project->viewpointCameraModel()}
    , m_extent{}
    , m_windowAspect{kDefaultWindowAspect}
    , m_projectionMatrix{Core::Matrix4x4::identity()}
{
    CHECK(m_project) << "GfxProject::ctor: Model::Project nullptr!";

    for (auto& modelObject : m_project->modelObjectList())
        this->add(modelObject.get());

    m_project->modelObjectAdded.connect<GfxProject, &GfxProject::add>(this);
}

GfxProject::~GfxProject()
{
    m_project->modelObjectAdded.disconnect<GfxProject, &GfxProject::add>(this);

    for (auto graphicsObject : m_graphicsObjectList) {
        graphicsObject->extentChanged.disconnect<GfxProject, &GfxProject::updateExtent>(this);
        delete graphicsObject;
    }
}

void GfxProject::adjustProjection(int width, int height)
{
    m_windowAspect = static_cast<double>(width) / static_cast<double>(height);
    this->updateProjectionMatrix();
}

void GfxProject::add(Model::Geometry* modelObject)
{
    if (!modelObject)
        return;

    GraphicsObject* graphicsObject = nullptr;

    switch (modelObject->type()) {
    case Model::Geometry::Type::Box:
        graphicsObject = new GfxBox{*this, dynamic_cast<Model::Box*>(modelObject)};
        break;
    case Model::Geometry::Type::Stl:
        graphicsObject = new GfxStl{*this, dynamic_cast<Model::Stl*>(modelObject)};
        break;
    }

    if (!graphicsObject)
        return;

    m_graphicsObjectList.push_back(graphicsObject);
    graphicsObject->extentChanged.connect<GfxProject, &GfxProject::updateExtent>(this);
    graphicsObjectAdded.emit_signal(*graphicsObject); // emit signal

    this->updateExtent();
}

void GfxProject::updateExtent()
{
    Extent extent;
    for (const auto& object : m_graphicsObjectList)
        extent.update(object->extent());

    if (m_extent != extent) {
        m_extent = extent;
        m_project->viewpointCameraModel().setViewportTranslation(m_extent.maxLength() * 2.0);
        this->updateProjectionMatrix();
        extentChanged.emit_signal(); // emit signal
    }
}

void GfxProject::updateProjectionMatrix()
{
    m_projectionMatrix = Core::Matrix4x4::perspective(50.0, m_windowAspect, 0.1, m_extent.maxLength() * 3.0);
}

} // namespace GfxModel
