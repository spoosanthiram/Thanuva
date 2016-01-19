/**
* Author: Saravanan Poosanthiram
* $LastChangedBy: ps $
* $LastChangedDate: 2015-03-20 18:08:01 -0400 (Fri, 20 Mar 2015) $
*/

#include "Geometry.h"

#include <boost/property_tree/ptree.hpp>

namespace {

const char* kMaterialTag = "material";
const char* kTransformMatrixTag = "transformMatrix";

} // anonymous

namespace Model {

const char* Geometry::kTypeTag = "type";

void Geometry::setMaterial(const Core::Material& material, Core::EmitSignal emitSignal)
{
    if (material == m_material)
        return;

    m_material = material;
    if (Core::EmitSignal::Emit == emitSignal)
        materialChanged.emit_signal(); // emit signal
}

void Geometry::setTransformMatrix(const Core::Matrix4x4& transformMatrix, Core::EmitSignal emitSignal)
{
    if (transformMatrix == m_transformMatrix)
        return;

    m_transformMatrix = transformMatrix;
    if (Core::EmitSignal::Emit == emitSignal)
        transformMatrixChanged.emit_signal(); // emit signal
}

void Geometry::loadGeometry(const boost::property_tree::ptree& geometryPropTree)
{
    m_material.set(geometryPropTree.get<std::string>(kMaterialTag));
    m_transformMatrix.set(geometryPropTree.get<std::string>(kTransformMatrixTag));
}

void Geometry::saveGeometry(boost::property_tree::ptree& geometryPropTree)
{
    geometryPropTree.put(kTypeTag, static_cast<int>(this->type()));
    geometryPropTree.put(kMaterialTag, m_material.str());
    geometryPropTree.put(kTransformMatrixTag, m_transformMatrix.str());
}

} // namespace Model
