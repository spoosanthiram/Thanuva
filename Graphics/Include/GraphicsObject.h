/*
 * Graphics: Graphical objects for Thanuva Model/Geometry obejcts
 *
 * Copyright 2016, Saravanan Poosanthiram
 * All rights reserved.
 */

#ifndef Graphics_GraphicsObject_h
#define Graphics_GraphicsObject_h

#include <vector>

#include <nano_signal_slot.hpp>

#include "OpenGLInterface.h"
#include "Point3d.h"

namespace Geometry { class Geometry; }

namespace Graphics {

class AbstractGraphicsScene;

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
    GraphicsObject(const AbstractGraphicsScene& graphicsScene, Geometry::Geometry* geometry);

    void render() const;
    bool probe(int x, int y) const;
    const std::vector<Core::Point3d>& probePoints() const;

public: // signals
    Nano::Signal<void()> graphicsObjectChanged;

private: // slots
    void emitGraphicsObjectChanged() { graphicsObjectChanged.emit_signal(); }

private:
    void initialize();
    Core::Point3d glNearPoint(int x, int y) const;
    Core::Point3d glFarPoint(int x, int y) const;

    const AbstractGraphicsScene& m_graphicsScene;
    Geometry::Geometry* m_geometry;
    GLuint m_vaoHandle{0};
    GLuint m_bufferHandle[kBufferSize];
};

} // namespace Graphics

#endif // Graphics_GraphicsObject_h
