/*
 * Graphics: Graphical objects for Thanuva Model/Geometry obejcts
 *
 * Copyright 2016, Saravanan Poosanthiram
 * All rights reserved.
 */

#ifndef GRAPHICS_GRAPHICSOBJECT_H
#define GRAPHICS_GRAPHICSOBJECT_H

#include <vector>

#include <nano_signal_slot.hpp>

#include "OpenGLInterface.h"
#include "Point3d.h"

namespace Geometry { class GeometryObject; }

namespace Graphics {

class GraphicsEnvironment;

class GraphicsObject
{
public:
    static const int kVertexBuffer = 0;
    static const int kNormalBuffer = 1;
    static const int kIndexBuffer = 2;

    static const int kBufferSize = 3;

    static const int kVertexLocation = 0;
    static const int kNormalLocation = 1;

public:
    GraphicsObject(const GraphicsEnvironment& graphicsEnvironment, Geometry::GeometryObject* geometryObject);

    void render() const;
    bool probe(int x, int y) const;
    const std::vector<Core::Point3d>& probePoints() const;

public: // signals
    Nano::Signal<void()> graphicsObjectChanged;

private:
    void initialize();
    Core::Point3d glNearPoint(int x, int y) const;
    Core::Point3d glFarPoint(int x, int y) const;

    const GraphicsEnvironment& m_graphicsEnvironment;
    Geometry::GeometryObject* m_geometryObject;
    GLuint m_vaoHandle{0};
    GLuint m_bufferHandle[kBufferSize];
};

} // namespace Graphics

#endif // GRAPHICS_GRAPHICSOBJECT_H
