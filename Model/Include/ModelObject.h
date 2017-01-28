/*
 * Model: Model objects for Thanuva
 *
 * Copyright 2016, Saravanan Poosanthiram
 * All rights reserved.
 */

#ifndef MODEL_MODELOBJECT_H
#define MODEL_MODELOBJECT_H

#include <boost/property_tree/ptree_fwd.hpp>
#include <nano_signal_slot.hpp>

#include "CoreDef.h"
#include "Material.h"
#include "Matrix4x4.h"

namespace Model {

class Scene;

class ModelObject
{
public:
    enum class Type
    {
        Box,
        Stl
    };
    static std::string typeStr(Type type);

    static const char* kTypeTag;

public:
    ModelObject(const Scene& scene) : m_scene{scene} {}
    ModelObject(const ModelObject& rhs) = delete; // TODO: needs to be implemented, for now deleted
    virtual ~ModelObject() {}

    ModelObject& operator=(const ModelObject& rhs) = delete; // // TODO: needs to be implemented, for now deleted

    virtual Type type() const = 0;
    const std::string& name() const { return m_name; }
    const Core::Material& material() const { return m_material; }
    const Core::Matrix4x4& transformMatrix() const { return m_transformMatrix; }
    std::string label() const;

    void setName(const std::string& name);
    void setMaterial(const Core::Material& material,
                     Core::EmitSignal emitSignal = Core::EmitSignal::Emit);
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
    const Scene& m_scene;

    std::string m_name{};
    Core::Material m_material{};
    Core::Matrix4x4 m_transformMatrix{Core::Matrix4x4::identity()};
};

} // namespace Model

#endif // MODEL_MODELOBJECT_H
