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
    CylinderModel(const Scene* scene);
    CylinderModel(const Scene* scene, const Core::Point3d& endpoint1, double radius1,
                  const Core::Point3d& endpoint2, double radius2, unsigned int numFacets);
    CylinderModel(const CylinderModel& rhs) = delete; // TODO: needs to be implemented

    CylinderModel& operator=(const CylinderModel& rhs) = delete; // TODO: needs to be implemented

    Type type() const override { return Type::Cylinder; }
    const Core::Point3d& endpoint1() const { return m_endpoint1; }
    double radius1() const { return m_radius1; }
    const Core::Point3d& endpoint2() const { return m_endpoint2; }
    double radius2() const { return m_radius2; }
    unsigned int numFacets() const { return m_numFacets; }

    void setEndpoint1(const Core::Point3d& point);
    void setRadius1(double radius);
    void setEndpoint2(const Core::Point3d& point);
    void setRadius2(double radius);
    void setNumFacets(unsigned int facets);

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
    void connectSignals();

    Core::Point3d m_endpoint1{0.0, 0.0, 0.0};
    double m_radius1{1.0};
    Core::Point3d m_endpoint2{0.0, 0.0, 1.0};
    double m_radius2{1.0};
    unsigned int m_numFacets{128};
};

} // namespace Model

#endif // MODEL_CYLINDERMODEL_H
