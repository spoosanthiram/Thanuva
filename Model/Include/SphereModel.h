/*
 * Model: Model objects for Thanuva
 *
 * Copyright 2016, Saravanan Poosanthiram
 * All rights reserved.
 */

#ifndef MODEL_SPHEREMODEL_H
#define MODEL_SPHEREMODEL_H

#include "Point3d.h"
#include "ModelObject.h"

namespace Model {

class SphereModel : public ModelObject
{
public:
    static const int kMaxSubdivisions = 5;
    static int numFacets(int subdivisions) { return static_cast<int>(8 * std::pow(4, subdivisions)); }

public:
    SphereModel(const Scene* scene);
    SphereModel(const Scene* scene, const Core::Point3d& center, double radius, unsigned int subdivisions);
    SphereModel(const SphereModel& rhs) = delete; // TODO: needs to be implemented

    SphereModel& operator=(const SphereModel& rhs) = delete; // TODO: needs to be implemented

    Type type() const override { return Type::Sphere; }
    const Core::Point3d& center() const { return m_center; }
    double radius() const { return m_radius; }
    unsigned int subdivisions() const { return m_subdivisions; }

    void setCenter(const Core::Point3d& center);
    void setRadius(double radius);
    void setSubdivisions(unsigned int subdivisions);

public: // signals
    Nano::Signal<void()> centerChanged{};
    Nano::Signal<void()> radiusChanged{};
    Nano::Signal<void()> subdivisionsChanged{};

protected:
    void loadModel(const boost::property_tree::ptree& modelPropTree) override;
    void saveModel(boost::property_tree::ptree& modelPropTree) override;

private:
    void connectSignals();

    Core::Point3d m_center{0.0, 0.0, 0.0};
    double m_radius{1.0};
    unsigned int m_subdivisions{2};
};

} // namespace Model

#endif // MODEL_SPHEREMODEL_H
