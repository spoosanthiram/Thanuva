/*
 * Model: Model objects for Thanuva
 *
 * Copyright 2016, Saravanan Poosanthiram
 * All rights reserved.
 */

#ifndef MODEL_MODELOBJECT_H
#define MODEL_MODELOBJECT_H

#include <AlgoBase.h>
#include <boost/property_tree/ptree_fwd.hpp>
#include <nano_signal_slot.hpp>

#include "CoreDef.h"
#include "Material.h"

namespace Model {

class Scene;

class ModelObject
{
public:
    enum class Type
    {
        Box,
        Mesh,
        Cylinder,
        Cone,
        Sphere,
        // Always add new types before this line!
        NTypes
    };
    static std::string typeStr(Type type);

    static const char* kTypeTag;

    struct Transform
    {
        Transform() : translateX{0.0}, translateY{0.0}, translateZ{0.0} {}
        bool operator==(const Transform& rhs)
        {
            return psa::isequal(translateX, rhs.translateX) &&
                psa::isequal(translateY, rhs.translateY) &&
                psa::isequal(translateZ, rhs.translateZ);
        }
        std::string str() const;
        void set(const std::string& str);

        double translateX;
        double translateY;
        double translateZ;
    };

public:
    ModelObject(const Scene* scene);
    ModelObject(const ModelObject& rhs) = delete; // TODO: needs to be implemented, for now deleted
    virtual ~ModelObject() {}

    ModelObject& operator=(const ModelObject& rhs) = delete; // // TODO: needs to be implemented, for now deleted

    virtual Type type() const = 0;
    const std::string& name() const { return m_name; }
    const Core::Material& material() const { return m_material; }
    const Transform& transform() const { return m_transform; }
    std::string label() const;

    void setName(const std::string& name);
    void setMaterial(const Core::Material& material);
    void setTransform(const Transform& transform);

    void load(const boost::property_tree::ptree& modelPropTree);
    void save(boost::property_tree::ptree& modelPropTree);

public: //slots
    void emitModelObjectChanged() { modelObjectChanged.emit_signal(this); }

public: // signals
    Nano::Signal<void()> nameChanged{};
    Nano::Signal<void()> materialChanged{};
    Nano::Signal<void()> transformChanged{};

    Nano::Signal<void(ModelObject*)> modelObjectChanged{}; // meant for most generic change, should be used sparingly

protected:
    virtual void loadModel(const boost::property_tree::ptree& /*modelPropTree*/) {}
    virtual void saveModel(boost::property_tree::ptree& /*modelPropTree*/) {}

private:
    const Scene* m_scene;

    std::string m_name{};
    Core::Material m_material{Core::Material::defaultMaterial()};
    Transform m_transform{};
};

} // namespace Model

#endif // MODEL_MODELOBJECT_H
