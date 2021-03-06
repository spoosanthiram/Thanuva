/*
 * Geometry: Geometry objects for Thanuva
 *
 * Copyright 2016, Saravanan Poosanthiram
 * All rights reserved.
 */

#ifndef Geometry_ThanuvaGeometry_h
#define Geometry_ThanuvaGeometry_h

#include <nano_signal_slot.hpp>

namespace Model { class ThanuvaModel; }

namespace Geometry {

class GeometryScene;

class ThanuvaGeometry
{
public:
    ThanuvaGeometry(const GeometryScene* geometryScene, Model::ThanuvaModel* thanuvaModel);

    const GeometryScene* geometryScene() const { return m_geometryScene; }
    Model::ThanuvaModel* thanuvaModel() const { return m_thanuvaModel; }

public: // signals
    Nano::Signal<void()> thanuvaGeometryChanged{}; // meant for most generic change, should be used sparingly

private:
    const GeometryScene* m_geometryScene;
    Model::ThanuvaModel* m_thanuvaModel;
};

} // namespace Geometry

#endif // Geometry_ThanuvaGeometry_h
