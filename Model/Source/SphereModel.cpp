/*
 * Model: Model objects for Thanuva
 *
 * Copyright 2016, Saravanan Poosanthiram
 * All rights reserved.
 */

#include "SphereModel.h"

#include <boost/property_tree/ptree.hpp>
#ifdef UNIT_TEST
#include <gtest/gtest.h>
#endif

#ifdef UNIT_TEST
#include "Scene.h"
#include "ThanuvaApp.h"
#endif

namespace {

const char* kCenterXTag = "center_x";
const char* kCenterYTag = "center_y";
const char* kCenterZTag = "center_z";
const char* kRadiusTag = "radius";
const char* kSubdivisionsTag = "subdivisions";

} // anonymous

namespace Model {

const char* SphereModel::kType = "Sphere";

SphereModel::SphereModel(const Scene* scene)
    : GeometryModel{scene}
{
    this->connectSignals();
}

SphereModel::SphereModel(const Scene* scene, const Core::Point3d& center,
                         double radius, unsigned int subdivisions)
    : GeometryModel{scene}
    , m_center{center}
    , m_radius{radius}
    , m_subdivisions{subdivisions}
{
    this->connectSignals();
}

void SphereModel::setCenter(const Core::Point3d& center)
{
    if (m_center == center)
        return;

    m_center = center;
    centerChanged.emit_signal();
}

void SphereModel::setRadius(double radius)
{
    if (psa::isequal(m_radius, radius))
        return;

    m_radius = radius;
    radiusChanged.emit_signal();
}

void SphereModel::setSubdivisions(unsigned int subdivisions)
{
    if (m_subdivisions == subdivisions)
        return;

    m_subdivisions = subdivisions;
    subdivisionsChanged.emit_signal();
}

void SphereModel::loadGeometryModel(const boost::property_tree::ptree& modelPropTree)
{
    m_center.setX(modelPropTree.get<double>(kCenterXTag));
    m_center.setY(modelPropTree.get<double>(kCenterYTag));
    m_center.setZ(modelPropTree.get<double>(kCenterZTag));

    m_radius = modelPropTree.get<double>(kRadiusTag);

    m_subdivisions = modelPropTree.get<unsigned int>(kSubdivisionsTag);
}

void SphereModel::saveGeometryModel(boost::property_tree::ptree& modelPropTree)
{
    modelPropTree.put(kCenterXTag, m_center.x());
    modelPropTree.put(kCenterYTag, m_center.y());
    modelPropTree.put(kCenterZTag, m_center.z());

    modelPropTree.put(kRadiusTag, m_radius);

    modelPropTree.put(kSubdivisionsTag, m_subdivisions);
}

void SphereModel::connectSignals()
{
    centerChanged.connect<ThanuvaModel, &ThanuvaModel::emitThanuvaModelChanged>(this);
    radiusChanged.connect<ThanuvaModel, &ThanuvaModel::emitThanuvaModelChanged>(this);
    subdivisionsChanged.connect<ThanuvaModel, &ThanuvaModel::emitThanuvaModelChanged>(this);
}

#ifdef UNIT_TEST

struct SphereTest : public ::testing::Test
{
    ThanuvaApp m_app;
    Scene* m_scene = m_app.newScene();
};

TEST_F(SphereTest, CreationDefault)
{
    SphereModel* sphereModel = m_scene->newModelObject<SphereModel>();
}

#endif // UNIT_TEST

} // namespace Model
