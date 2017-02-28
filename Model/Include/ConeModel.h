/*
 * Model: Model objects for Thanuva
 *
 * Copyright 2016, Saravanan Poosanthiram
 * All rights reserved.
 */

#ifndef MODEL_CONEMODEL_H
#define MODEL_CONEMODEL_H

#include "Point3d.h"
#include "ModelObject.h"

namespace Model {

class ConeModel : public ModelObject
{
public:
    ConeModel(const Scene* scene);
    ConeModel(const Scene* scene, const Core::Point3d& apex, const Core::Point3d& center,
              double radius, unsigned int numFacets);
    ConeModel(const ConeModel& rhs) = delete; // TODO: needs to be implemented

    ConeModel& operator=(const ConeModel& rhs) = delete; // TODO: needs to be implemented

    Type type() const override { return Type::Cone; }
    const Core::Point3d& apex() const { return m_apex; }
    const Core::Point3d& center() const { return m_center; }
    double radius() const { return m_radius; }
    unsigned int numFacets() const { return m_numFacets; }

    void setApex(const Core::Point3d& apex);
    void setCenter(const Core::Point3d& center);
    void setRadius(double radius);
    void setNumFacets(unsigned int facets);

public: // signals
    Nano::Signal<void()> apexChanged{};
    Nano::Signal<void()> centerChanged{};
    Nano::Signal<void()> radiusChanged{};
    Nano::Signal<void()> numFacetsChanged{};

protected:
    void loadModel(const boost::property_tree::ptree& modelPropTree) override;
    void saveModel(boost::property_tree::ptree& modelPropTree) override;

private:
    void connectSignals();

    Core::Point3d m_apex{0.0, 0.0, 1.0};
    Core::Point3d m_center{0.0, 0.0, 0.0};
    double m_radius{1.0};
    unsigned int m_numFacets{128};
};

} // namespace Model

#endif // MODEL_CONEMODEL_H
