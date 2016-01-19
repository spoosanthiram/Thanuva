/**
 * Author: Saravanan Poosanthiram
 * $LastChangedBy: ps $
 * $LastChangedDate: 2015-03-19 18:15:00 -0400 (Thu, 19 Mar 2015) $
 */

#ifndef MODEL_GEOMETRY_H
#define MODEL_GEOMETRY_H

#include <vector>

#include "CoreDef.h"
#include "Material.h"
#include "Matrix4x4.h"

#include <boost/property_tree/ptree_fwd.hpp>

#include "nano_signal_slot.hpp"

namespace Model {

class Project;

class Geometry {
public:
    enum class Type {
        Box,
        Stl
    };

    static const char* kTypeTag;

public:
    Geometry(const Project& project)
        : geometryChanged{}
        , m_project{project}
        , m_material{}
        , m_transformMatrix{Core::Matrix4x4::identity()}
    {}
    Geometry(const Geometry& rhs) = delete; // TODO: needs to be implemented, for now deleted
    virtual ~Geometry() {}

    Geometry& operator=(const Geometry& rhs) = delete; // // TODO: needs to be implemented, for now deleted

    virtual Type type() const = 0;
    const Core::Material& material() const { return m_material; }
    const Core::Matrix4x4& transformMatrix() const { return m_transformMatrix; }

    void setMaterial(const Core::Material& material, Core::EmitSignal emitSignal = Core::EmitSignal::Emit);
    void setTransformMatrix(const Core::Matrix4x4& transformMatrix,
                            Core::EmitSignal emitSignal = Core::EmitSignal::Emit);

    virtual void load(const boost::property_tree::ptree& geometryPropTree) = 0;
    virtual void save(boost::property_tree::ptree& geometryPropTree) = 0;

public: // signals
    Nano::Signal<void()> geometryChanged;
    Nano::Signal<void()> materialChanged;
    Nano::Signal<void()> transformMatrixChanged;

protected:
    void loadGeometry(const boost::property_tree::ptree& geometryPropTree);
    void saveGeometry(boost::property_tree::ptree& geometryPropTree);

private:
    const Project& m_project;

    Core::Material m_material;
    Core::Matrix4x4 m_transformMatrix;
};

} // namespace Model

#endif // MODEL_GEOMETRY_H
