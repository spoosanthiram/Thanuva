/**
* Author: Saravanan Poosanthiram
* $LastChangedBy: ps $
* $LastChangedDate: 2015-03-29 02:53:27 -0400 (Sun, 29 Mar 2015) $
*/

#ifndef GFXMODEL_GFXBOX_H
#define GFXMODEL_GFXBOX_H

#include "GraphicsObject.h"

namespace Model { class Box; }

namespace GfxModel {

class GfxBox : public GraphicsObject {
public:
    GfxBox(const GfxProject& gfxProject, Model::Box* box);

    bool intersect(const Core::Vector3d& nearPoint, const Core::Vector3d& farPoint, std::vector<Core::Vector3d>* points) override;

protected: // slots
    void initialize();
};

} // namespace GfxModel

#endif // GFXMODEL_GFXBOX_H
