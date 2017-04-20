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

const char* ConeModel::kType = "Cone";

ConeModel::ConeModel(const Scene* scene)
    : GeometryModel{scene}
{
    this->connectSignals();
}

ConeModel::ConeModel(const Scene* scene, const Core::Point3d& apex, const Core::Point3d& center,
                     double radius, unsigned int numFacets)
    : GeometryModel{scene}
    , m_apex{apex}
    , m_center{center}
    , m_radius{radius}
    , m_numFacets{numFacets}
{
    this->connectSignals();
}

void ConeModel::setApex(const Core::Point3d& apex)
{
    if (m_apex == apex)
        return;

    m_apex = apex;
    apexChanged.emit_signal();
}

void ConeModel::setCenter(const Core::Point3d& center)
{
    if (m_center == center)
        return;

    m_center = center;
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

void ConeModel::loadGeometryModel(const boost::property_tree::ptree& modelPropTree)
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

void ConeModel::saveGeometryModel(boost::property_tree::ptree& modelPropTree)
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

void ConeModel::connectSignals()
{
    apexChanged.connect<ThanuvaModel, &ThanuvaModel::emitThanuvaModelChanged>(this);
    centerChanged.connect<ThanuvaModel, &ThanuvaModel::emitThanuvaModelChanged>(this);
    radiusChanged.connect<ThanuvaModel, &ThanuvaModel::emitThanuvaModelChanged>(this);
    numFacetsChanged.connect<ThanuvaModel, &ThanuvaModel::emitThanuvaModelChanged>(this);
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
