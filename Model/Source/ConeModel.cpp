/*
 * Model: Model objects for Thanuva
 *
 * Copyright 2016, Saravanan Poosanthiram
 * All rights reserved.
 */

#include "ConeModel.h"

#include <boost/property_tree/ptree.hpp>
#ifdef UNIT_TEST
#include <gtest/gtest.h>
#endif

#ifdef UNIT_TEST
#include "Scene.h"
#include "ThanuvaApp.h"
#endif

namespace {

const char* kApexXTag = "apex_x";
const char* kApexYTag = "apex_y";
const char* kApexZTag = "apex_z";
const char* kCenterXTag = "center_x";
const char* kCenterYTag = "center_y";
const char* kCenterZTag = "center_z";
const char* kRadiusTag = "radius";
const char* kNumFacetsTag = "numfacets";

} // anonymous

namespace Model {

ConeModel::ConeModel(const Scene* scene)
    : ModelObject{scene}
{
    apexChanged.connect<ModelObject, &ModelObject::emitModelObjectChanged>(this);
    centerChanged.connect<ModelObject, &ModelObject::emitModelObjectChanged>(this);
    radiusChanged.connect<ModelObject, &ModelObject::emitModelObjectChanged>(this);
    numFacetsChanged.connect<ModelObject, &ModelObject::emitModelObjectChanged>(this);
}

void ConeModel::setApex(const Core::Point3d& point)
{
    if (m_apex == point)
        return;

    m_apex = point;
    apexChanged.emit_signal();
}

void ConeModel::setCenter(const Core::Point3d& point)
{
    if (m_center == point)
        return;

    m_center = point;
    centerChanged.emit_signal();
}

void ConeModel::setRadius(double radius)
{
    if (psa::isequal(m_radius, radius))
        return;

    m_radius = radius;
    radiusChanged.emit_signal();
}

void ConeModel::setNumFacets(unsigned int facets)
{
    if (m_numFacets == facets)
        return;

    m_numFacets = facets;
    numFacetsChanged.emit_signal();
}

void ConeModel::loadModel(const boost::property_tree::ptree& modelPropTree)
{
    m_apex.setX(modelPropTree.get<double>(kApexXTag));
    m_apex.setY(modelPropTree.get<double>(kApexYTag));
    m_apex.setZ(modelPropTree.get<double>(kApexZTag));

    m_center.setX(modelPropTree.get<double>(kCenterXTag));
    m_center.setY(modelPropTree.get<double>(kCenterYTag));
    m_center.setZ(modelPropTree.get<double>(kCenterZTag));

    m_radius = modelPropTree.get<double>(kRadiusTag);

    m_numFacets = modelPropTree.get<unsigned int>(kNumFacetsTag);
}

void ConeModel::saveModel(boost::property_tree::ptree& modelPropTree)
{
    modelPropTree.put(kApexXTag, m_apex.x());
    modelPropTree.put(kApexYTag, m_apex.y());
    modelPropTree.put(kApexZTag, m_apex.z());

    modelPropTree.put(kCenterXTag, m_center.x());
    modelPropTree.put(kCenterYTag, m_center.y());
    modelPropTree.put(kCenterZTag, m_center.z());

    modelPropTree.put(kRadiusTag, m_radius);

    modelPropTree.put(kNumFacetsTag, m_numFacets);
}

#ifdef UNIT_TEST

struct ConeTest : public ::testing::Test
{
    ThanuvaApp m_app;
    Scene* m_scene = m_app.newScene();
};

TEST_F(ConeTest, CreationDefault)
{
    ConeModel* coneModel = m_scene->newModelObject<ConeModel>();
}

#endif // UNIT_TEST

} // namespace Model
