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
    class Project;
}

namespace Geometry {

class GeometryContainer
{
public:
    GeometryContainer(Model::Project* project);
    GeometryContainer(const GeometryContainer& other) = delete;
    ~GeometryContainer();

    GeometryContainer& operator=(const GeometryContainer& other) = delete;

    const std::vector<GeometryObject*>& geometryObjectList() const { return m_geometryObjectList; }
    const Extent& extent() const { return m_extent; }
    void add(Model::ModelObject* modelObject);

public: // signals
    Nano::Signal<void(GeometryObject&)> geometryObjectAdded{};
    Nano::Signal<void()> extentChanged{};

protected: // slots
    void updateExtent();

private:
    Model::Project* m_project;

    std::vector<GeometryObject*> m_geometryObjectList{};
    Extent m_extent;
};

} // namespace Geometry

#endif // GEOMETRY_GEOMETRYCONTAINER_H
