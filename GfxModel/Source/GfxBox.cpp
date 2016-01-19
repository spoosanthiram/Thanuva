/**
* Author: Saravanan Poosanthiram
* $LastChangedBy: ps $
* $LastChangedDate: 2015-03-29 02:53:27 -0400 (Sun, 29 Mar 2015) $
*/

#include "GfxBox.h"

#include <chrono>

#include "glog/logging.h"
#include "nano_signal_slot.hpp"

#include "Box.h"

namespace GfxModel {

GfxBox::GfxBox(const GfxProject& gfxProject, Model::Box* box)
    : GraphicsObject(gfxProject, box)
{
    CHECK(box) << "GfxBox::ctor: Model::Box nullptr!";

    this->initialize();

    box->geometryChanged.connect<GfxBox, &GfxBox::initialize>(this);
}

bool GfxBox::intersect(const Core::Vector3d& nearPoint, const Core::Vector3d& farPoint, std::vector<Core::Vector3d>* points)
{
    const std::vector<float>& vertices = this->vertices();
    const std::vector<float>& normals = this->normals();
    const std::vector<int>& indices = this->indices();

    if (vertices.size() <= 0 || normals.size() <= 0 || indices.size() <= 0)
        return false;

    bool found = false;

    Core::Vector3d n, a, b, c, p;
    Core::Vector3d l = farPoint - nearPoint;

    size_t len = indices.size();
    for (size_t i = 0; i < len; i += 3) {
        a.assign(&vertices[indices[i] * 3]);
        n.assign(&normals[indices[i] * 3]);

        if (!this->GraphicsObject::intersect(a, n, nearPoint, l, p))
            continue;

        b.assign(&vertices[indices[i + 1] * 3]);
        c.assign(&vertices[indices[i + 2] * 3]);

        // is the point p inside the triangle?
        if (n.dot((b - a).cross(p - a)) >= 0.0 && n.dot((c - b).cross(p - b)) >= 0.0 && n.dot((a - c).cross(p - c)) >= 0.0) {
            found = true;
            if (points)
                points->push_back(p);
            else
                break;
        }
    }

    return found;
}

void GfxBox::initialize()
{
    this->clear();

    this->setExtent(Extent{
        std::numeric_limits<double>::infinity(),
        -std::numeric_limits<double>::infinity(),
        std::numeric_limits<double>::infinity(),
        -std::numeric_limits<double>::infinity(),
        std::numeric_limits<double>::infinity(),
        -std::numeric_limits<double>::infinity()
    });

    this->reserve(24 * GraphicsObject::kValuesPerVertex, 24 * GraphicsObject::kValuesPerVertex, 36);

    auto box = dynamic_cast<Model::Box*>(this->geometry());
    const Model::Box::Limiter& limiter = box->limiter();

    Core::Vector3d a{limiter.xlow, limiter.ylow, limiter.zlow};
    Core::Vector3d b{limiter.xhigh, limiter.ylow, limiter.zlow};
    Core::Vector3d c{limiter.xhigh, limiter.yhigh, limiter.zlow};
    Core::Vector3d d{limiter.xlow, limiter.yhigh, limiter.zlow};
    Core::Vector3d e{limiter.xlow, limiter.ylow, limiter.zhigh};
    Core::Vector3d f{limiter.xhigh, limiter.ylow, limiter.zhigh};
    Core::Vector3d g{limiter.xhigh, limiter.yhigh, limiter.zhigh};
    Core::Vector3d h{limiter.xlow, limiter.yhigh, limiter.zhigh};

    this->insertQuad(b, c, g, f); // X Max plane
    this->insertQuad(d, a, e, h); // X Min plane

    this->insertQuad(c, d, h, g); // Y Max plane
    this->insertQuad(a, b, f, e); // Y Min plane

    this->insertQuad(e, f, g, h); // Z Max plane
    this->insertQuad(d, c, b, a); // Z Min plane

    this->initializeBoundingBox();

    // emit signals
    graphicsObjectChanged.emit_signal();
    extentChanged.emit_signal();
}

} // namespace GfxModel
