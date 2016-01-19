/**
* Author: Saravanan Poosanthiram
* $LastChangedBy: ps $
* $LastChangedDate: 2015-03-29 02:53:27 -0400 (Sun, 29 Mar 2015) $
*/

#ifndef GLVIEWER_GLVIEWOBJECT_H
#define GLVIEWER_GLVIEWOBJECT_H

#include <vector>

#include "nano_signal_slot.hpp"

#include "Vector3d.h"

namespace GfxModel { class GraphicsObject; }

namespace GlViewer {

class GlProject;

class GlViewObject : public Nano::Observer {
public:
    static const int kVertexBuffer = 0;
    static const int kNormalBuffer = 1;
    static const int kIndexBuffer = 2;

    static const int kBufferSize = 3;

    static const int kVertexLocation = 0;
    static const int kNormalLocation = 1;

public:
    GlViewObject(const GlProject& glProject, GfxModel::GraphicsObject& graphicsObject);

    void render() const;
    bool probe(int x, int y) const;
    std::vector<Core::Vector3d> probePoints(int x, int y) const;

public: // signals
    Nano::Signal<void()> glViewObjectChanged;

private:
    void initialize();
    Core::Vector3d glNearPoint(int x, int y) const;
    Core::Vector3d glFarPoint(int x, int y) const;

    const GlProject& m_glProject;
    GfxModel::GraphicsObject& m_graphicsObject;
    GLuint m_vaoHandle;
    GLuint m_bufferHandle[kBufferSize];
};

} // namespace GlViewer

#endif // GLVIEWER_GLVIEWOBJECT_H
