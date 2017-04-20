/*
 * Model: Model objects for Thanuva
 *
 * Copyright 2016, Saravanan Poosanthiram
 * All rights reserved.
 */

#ifndef Model_CoordinateSystemModel_h
#define Model_CoordinateSystemModel_h

#include "Point3d.h"
#include "ThanuvaModel.h"

namespace Model {

class CoordinateSystemModel : public ThanuvaModel
{
public:
    CoordinateSystemModel(const Scene* scene);
    CoordinateSystemModel(const Scene* scene, const std::string& name);

    CoordinateSystemModel* parent() const { return m_parent; }
    Core::Point3d origin() const { return m_origin; }
    Core::Vector3d xAxis() const { return m_xAxis; }
    Core::Vector3d yAxis() const { return m_yAxis; }

    void setOrigin(const Core::Point3d& origin);
    void setXAxis(const Core::Vector3d& xAxis);
    void SetYAxis(const Core::Vector3d& yAxis);

public: // signals
    Nano::Signal<void()> originChanged;
    Nano::Signal<void()> xAxisChanged;
    Nano::Signal<void()> yAxisChanged;

protected:
    void loadModel(const boost::property_tree::ptree& modelPropTree) override;
    void saveModel(boost::property_tree::ptree& modelPropTree) override;

private:
    CoordinateSystemModel* m_parent{nullptr};
    Core::Point3d m_origin{0.0, 0.0, 0.0};
    Core::Vector3d m_xAxis{1.0, 0.0, 0.0};
    Core::Vector3d m_yAxis{0.0, 1.0, 0.0};
};

} // namespace Model

#endif // Model_CoordinateSystemModel_h
