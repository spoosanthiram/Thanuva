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

class SceneGeometry;

class ThanuvaGeometry
{
public:
    ThanuvaGeometry(const SceneGeometry* sceneGeometry, Model::ThanuvaModel* thanuvaModel);

    const SceneGeometry* sceneGeometry() const { return m_sceneGeometry; }
    Model::ThanuvaModel* thanuvaModel() const { return m_thanuvaModel; }

public: // signals
    Nano::Signal<void()> thanuvaGeometryChanged{}; // meant for most generic change, should be used sparingly

private:
    const SceneGeometry* m_sceneGeometry;
    Model::ThanuvaModel* m_thanuvaModel;
};

} // namespace Geometry

#endif // Geometry_ThanuvaGeometry_h
