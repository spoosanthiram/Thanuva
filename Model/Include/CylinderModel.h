/*
 * Model: Model objects for Thanuva
 *
 * Copyright 2016, Saravanan Poosanthiram
 * All rights reserved.
 */

#ifndef MODEL_CYLINDERMODEL_H
#define MODEL_CYLINDERMODEL_H

#include "Point3d.h"
#include "ModelObject.h"

namespace Model {

class CylinderModel : public ModelObject
{
public:
    CylinderModel(const Scene& scene) : ModelObject{scene} {}
    CylinderModel(const CylinderModel& rhs) = delete; // TODO: needs to be implemented

    CylinderModel& operator=(const CylinderModel& rhs) = delete; // TODO: needs to be implemented

    Type type() const override { return Type::Cylinder; }

public: // signals
    Nano::Signal<void()> endpoint1Changed{};
    Nano::Signal<void()> radius1Changed{};
    Nano::Signal<void()> endpoint2Changed{};
    Nano::Signal<void()> radius2Changed{};
    Nano::Signal<void()> numFacetsChanged{};

protected:
    void loadModel(const boost::property_tree::ptree& modelPropTree) override;
    void saveModel(boost::property_tree::ptree& modelPropTree) override;

private:
    Core::Point3d m_endpoint1;
    double m_radius1;
    Core::Point3d m_endpoint2;
    double m_radius2;
    unsigned int m_numFacets;
};

} // namespace Model

#endif // MODEL_CYLINDERMODEL_H
