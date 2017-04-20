/*
 * Model: Model objects for Thanuva
 *
 * Copyright 2016, Saravanan Poosanthiram
 * All rights reserved.
 */

#include "CoordinateSystemModel.h"

#include <boost/property_tree/ptree.hpp>

#include "Vector3d.h"

namespace {

const char* kOriginTag = "origin";
const char* kXAxisTag = "x_axis";
const char* kYAxisTag = "y_axis";

} // anonymous

namespace Model {

CoordinateSystemModel::CoordinateSystemModel(const Scene* scene)
    : ThanuvaModel{scene}
{
}

CoordinateSystemModel::CoordinateSystemModel(const Scene* scene, const std::string& name)
    : ThanuvaModel{scene}
{
    this->setName(name);
}

void CoordinateSystemModel::setOrigin(const Core::Point3d& origin)
{
    if (m_origin == origin)
        return;

    m_origin = origin;
    originChanged.emit_signal(); // emit signal
}

void CoordinateSystemModel::setXAxis(const Core::Vector3d& xAxis)
{
    if (m_xAxis == xAxis)
        return;

    m_xAxis = xAxis;
    xAxisChanged.emit_signal(); // emit signal
}

void CoordinateSystemModel::SetYAxis(const Core::Vector3d& yAxis)
{
    if (m_yAxis == yAxis)
        return;

    m_yAxis = yAxis;
    yAxisChanged.emit_signal(); // emit signal
}

void CoordinateSystemModel::loadModel(const boost::property_tree::ptree& modelPropTree)
{
    m_origin.set(modelPropTree.get<std::string>(kOriginTag));
    m_xAxis.set(modelPropTree.get<std::string>(kXAxisTag));
    m_yAxis.set(modelPropTree.get<std::string>(kYAxisTag));
}

void CoordinateSystemModel::saveModel(boost::property_tree::ptree& modelPropTree)
{
    modelPropTree.put(kOriginTag, m_origin.str());
    modelPropTree.put(kXAxisTag, m_xAxis.str());
    modelPropTree.put(kYAxisTag, m_yAxis.str());
}

} // namespace Model
