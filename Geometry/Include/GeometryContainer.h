/*
 * Geometry: Geometry objects for Thanuva
 *
 * Copyright 2016, Saravanan Poosanthiram
 * All rights reserved.
 */

#ifndef GEOMETRY_GEOMETRYCONTAINER_H
#define GEOMETRY_GEOMETRYCONTAINER_H

#include <memory>
#include <vector>

#include <nano_signal_slot.hpp>

#include "GeometryObject.h"

namespace Model {
    class ModelObject;
    class Scene;
}

namespace Geometry {

class GeometryContainer
{
public:
    GeometryContainer(Model::Scene* scene);
    GeometryContainer(const GeometryContainer& other) = delete;
    ~GeometryContainer();

    GeometryContainer& operator=(const GeometryContainer& other) = delete;

    const std::vector<std::unique_ptr<GeometryObject>>& geometryObjectList() const
    {
        return m_geometryObjectList;
    }
    const Extent& extent() const { return m_extent; }
    void add(Model::ModelObject* modelObject);

public: // signals
    Nano::Signal<void(GeometryObject*)> geometryObjectAdded{};
    Nano::Signal<void()> extentChanged{};

private: // slots
    void updateExtent();

private:
    std::unique_ptr<GeometryObject> makeGeometryObject(Model::ModelObject* modelObject);

    Model::Scene* m_scene;

    std::vector<std::unique_ptr<GeometryObject>> m_geometryObjectList{};
    Extent m_extent;
};

} // namespace Geometry

#endif // GEOMETRY_GEOMETRYCONTAINER_H
