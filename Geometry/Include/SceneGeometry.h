/*
 * Geometry: Geometry objects for Thanuva
 *
 * Copyright 2016, Saravanan Poosanthiram
 * All rights reserved.
 */

#ifndef Geometry_SceneGeometry_h
#define Geometry_SceneGeometry_h

#include <memory>
#include <vector>

#include <nano_signal_slot.hpp>

#include "CoordinateSystem.h"
#include "Geometry.h"

namespace Model {
class CoordinateSystemModel;
class GeometryModel;
class Scene;
}

namespace Geometry {

class SceneGeometry
{
public:
    SceneGeometry(Model::Scene* scene);
    SceneGeometry(const SceneGeometry& other) = delete;
    ~SceneGeometry();

    SceneGeometry& operator=(const SceneGeometry& other) = delete;

    const std::vector<std::unique_ptr<CoordinateSystem>>& coordinateSystemList() const
    {
        return m_coordinateSystemList;
    }
    const std::vector<std::unique_ptr<Geometry>>& geometryObjectList() const { return m_geometryList; }
    const Extent& extent() const { return m_extent; }
    const CoordinateSystem* coordinateSystem(const Model::CoordinateSystemModel* csysModel) const;

    void add(Model::CoordinateSystemModel* csysModel);
    void add(Model::GeometryModel* geometryModel);

public: // signals
    Nano::Signal<void(Geometry*)> geometryAdded{};
    Nano::Signal<void()> extentChanged{};

private: // slots
    void updateExtent();

private:
    std::unique_ptr<Geometry> makeGeometry(Model::GeometryModel* geometryModel);

    Model::Scene* m_scene;

    std::vector<std::unique_ptr<CoordinateSystem>> m_coordinateSystemList{};
    std::vector<std::unique_ptr<Geometry>> m_geometryList{};
    Extent m_extent;
};

} // namespace Geometry

#endif // Geometry_SceneGeometry_h
