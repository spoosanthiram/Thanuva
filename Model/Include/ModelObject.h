/*
 * Model: Model objects for Thanuva
 *
 * Copyright 2016, Saravanan Poosanthiram
 * All rights reserved.
 */

#ifndef MODEL_MODELOBJECT_H
#define MODEL_MODELOBJECT_H

#include <vector>

#include <boost/property_tree/ptree_fwd.hpp>
#include <nano_signal_slot.hpp>

#include "CoreDef.h"
#include "Material.h"
#include "Matrix4x4.h"

namespace Model {

class Project;

class ModelObject {
public:
    enum class Type {
        Box,
        Stl
    };

    static const char* kTypeTag;

public:
    ModelObject(const Project& project) : m_project{project} {}
    ModelObject(const ModelObject& rhs) = delete; // TODO: needs to be implemented, for now deleted
    virtual ~ModelObject() {}

    ModelObject& operator=(const ModelObject& rhs) = delete; // // TODO: needs to be implemented, for now deleted

    virtual Type type() const = 0;
    const Core::Material& material() const { return m_material; }
    const Core::Matrix4x4& transformMatrix() const { return m_transformMatrix; }

    void setMaterial(const Core::Material& material, Core::EmitSignal emitSignal = Core::EmitSignal::Emit);
    void setTransformMatrix(const Core::Matrix4x4& transformMatrix,
                            Core::EmitSignal emitSignal = Core::EmitSignal::Emit);

    void load(const boost::property_tree::ptree& modelPropTree);
    void save(boost::property_tree::ptree& modelPropTree);

public: // signals
    Nano::Signal<void()> modelObjectChanged{};
    Nano::Signal<void()> materialChanged{};
    Nano::Signal<void()> transformMatrixChanged{};

protected:
    virtual void loadModel(const boost::property_tree::ptree& /*modelPropTree*/) {}
    virtual void saveModel(boost::property_tree::ptree& /*modelPropTree*/) {}

private:
    const Project& m_project;

    Core::Material m_material{};
    Core::Matrix4x4 m_transformMatrix{Core::Matrix4x4::identity()};
};

} // namespace Model

#endif // MODEL_MODELOBJECT_H
