/*
 * Model: Model objects for Thanuva
 *
 * Copyright 2016, Saravanan Poosanthiram
 * All rights reserved.
 */

#ifndef Model_ThanuvaModel_h
#define Model_ThanuvaModel_h

#include <AlgoBase.h>
#include <boost/property_tree/ptree_fwd.hpp>
#include <nano_signal_slot.hpp>

#include "CoreDef.h"

namespace Model {

class Scene;

class ThanuvaModel
{
public:
    ThanuvaModel(const Scene* scene);
    ThanuvaModel(const ThanuvaModel& rhs) = delete; // TODO: needs to be implemented, for now deleted
    virtual ~ThanuvaModel() {}

    ThanuvaModel& operator=(const ThanuvaModel& rhs) = delete; // // TODO: needs to be implemented, for now deleted

    const Scene* scene() const { return m_scene; }
    const std::string& name() const { return m_name; }

    void setName(const std::string& name);

    void load(const boost::property_tree::ptree& modelPropTree);
    void save(boost::property_tree::ptree& modelPropTree);

public: //slots
    void emitThanuvaModelChanged() { thanuvaModelChanged.emit_signal(this); }

public: // signals
    Nano::Signal<void()> nameChanged{};
    Nano::Signal<void(ThanuvaModel*)> thanuvaModelChanged{}; // meant for most generic change, should be used sparingly

protected:
    virtual void loadModel(const boost::property_tree::ptree& /*modelPropTree*/) = 0;
    virtual void saveModel(boost::property_tree::ptree& /*modelPropTree*/) = 0;

private:
    const Scene* m_scene;
    std::string m_name{};
};

} // namespace Model

#endif // Model_ThanuvaModel_h
