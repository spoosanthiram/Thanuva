/**
* Author: Saravanan Poosanthiram
* $LastChangedBy: ps $
* $LastChangedDate: 2015-03-29 02:53:27 -0400 (Sun, 29 Mar 2015) $
*/

#ifndef GFXMODEL_GFXSTL_H
#define GFXMODEL_GFXSTL_H

#include "GraphicsObject.h"

namespace Model { class Stl; }

namespace GfxModel {

class GfxStl : public GraphicsObject {
public:
    static const int kBinaryHeaderLength = 80;
    static const int kDataStreamReadBufferSize = 2097152;
    static const int kFacetSize = 12 * sizeof(float) + sizeof(uint16_t); // 1 normal and three vertices = 12 floats; uint16_t for pad
    static const int kNFacetChunk = 10000;

public:
    GfxStl(const GfxProject& gfxProject, Model::Stl* stl);

    bool intersect(const Core::Vector3d& nearPoint, const Core::Vector3d& farPoint, std::vector<Core::Vector3d>* points) override;

protected: // slots
    void initialize();
};

} // namespace GfxModel

#endif // GFXMODEL_GFXSTL_H
