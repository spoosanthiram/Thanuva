/*
 * Model: Model objects for Thanuva
 *
 * Copyright 2016, Saravanan Poosanthiram
 * All rights reserved.
 */

#include "CylinderModel.h"

#include <boost/property_tree/ptree.hpp>
#ifdef UNIT_TEST
#include <gtest/gtest.h>
#endif

#ifdef UNIT_TEST
#include "Scene.h"
#include "ThanuvaApp.h"
#endif

namespace {

const char* kEndpoint1XTag = "endpont1x";
const char* kEndpoint1YTag = "endpont1y";
const char* kEndpoint1ZTag = "endpont1z";
const char* kRadius1Tag = "radius1";
const char* kEndpoint2XTag = "endpont2x";
const char* kEndpoint2YTag = "endpont2y";
const char* kEndpoint2ZTag = "endpont2z";
const char* kRadius2Tag = "radius2";
const char* kNumFacetsTag = "numfacets";

} // anonymous

namespace Model {

const char* CylinderModel::kType = "Cylinder";

CylinderModel::CylinderModel(const Scene* scene)
    : GeometryModel{scene}
{
    this->connectSignals();
}

CylinderModel::CylinderModel(const Scene* scene, const Core::Point3d& endpoint1, double radius1,
                             const Core::Point3d& endpoint2, double radius2, unsigned int numFacets)
    : GeometryModel{scene}
    , m_endpoint1{endpoint1}
    , m_radius1{radius1}
    , m_endpoint2{endpoint2}
    , m_radius2{radius2}
    , m_numFacets{numFacets}
{
    this->connectSignals();
}

void CylinderModel::setEndpoint1(const Core::Point3d& point)
{
    if (m_endpoint1 == point)
        return;

    m_endpoint1 = point;
    endpoint1Changed.emit_signal();
}

void CylinderModel::setRadius1(double radius)
{
    if (psa::isequal(m_radius1, radius))
        return;

    m_radius1 = radius;
    radius1Changed.emit_signal();
}

void CylinderModel::setEndpoint2(const Core::Point3d& point)
{
    if (m_endpoint2 == point)
        return;

    m_endpoint2 = point;
    endpoint2Changed.emit_signal();
}

void CylinderModel::setRadius2(double radius)
{
    if (psa::isequal(m_radius2, radius))
        return;

    m_radius2 = radius;
    radius2Changed.emit_signal();
}

void CylinderModel::setNumFacets(unsigned int facets)
{
    if (m_numFacets == facets)
        return;

    m_numFacets = facets;
    numFacetsChanged.emit_signal();
}

void CylinderModel::loadGeometryModel(const boost::property_tree::ptree& modelPropTree)
{
    m_endpoint1.setX(modelPropTree.get<double>(kEndpoint1XTag));
    m_endpoint1.setY(modelPropTree.get<double>(kEndpoint1YTag));
    m_endpoint1.setZ(modelPropTree.get<double>(kEndpoint1ZTag));

    m_radius1 = modelPropTree.get<double>(kRadius1Tag);

    m_endpoint2.setX(modelPropTree.get<double>(kEndpoint2XTag));
    m_endpoint2.setY(modelPropTree.get<double>(kEndpoint2YTag));
    m_endpoint2.setZ(modelPropTree.get<double>(kEndpoint2ZTag));

    m_radius2 = modelPropTree.get<double>(kRadius2Tag);

    m_numFacets = modelPropTree.get<unsigned int>(kNumFacetsTag);
}

void CylinderModel::saveGeometryModel(boost::property_tree::ptree& modelPropTree)
{
    modelPropTree.put(kEndpoint1XTag, m_endpoint1.x());
    modelPropTree.put(kEndpoint1YTag, m_endpoint1.y());
    modelPropTree.put(kEndpoint1ZTag, m_endpoint1.z());

    modelPropTree.put(kRadius1Tag, m_radius1);

    modelPropTree.put(kEndpoint2XTag, m_endpoint2.x());
    modelPropTree.put(kEndpoint2YTag, m_endpoint2.y());
    modelPropTree.put(kEndpoint2ZTag, m_endpoint2.z());

    modelPropTree.put(kRadius2Tag, m_radius2);

    modelPropTree.put(kNumFacetsTag, m_numFacets);
}

void CylinderModel::connectSignals()
{
    endpoint1Changed.connect<ThanuvaModel, &ThanuvaModel::emitThanuvaModelChanged>(this);
    radius1Changed.connect<ThanuvaModel, &ThanuvaModel::emitThanuvaModelChanged>(this);
    endpoint2Changed.connect<ThanuvaModel, &ThanuvaModel::emitThanuvaModelChanged>(this);
    radius2Changed.connect<ThanuvaModel, &ThanuvaModel::emitThanuvaModelChanged>(this);
    numFacetsChanged.connect<ThanuvaModel, &ThanuvaModel::emitThanuvaModelChanged>(this);
}

#ifdef UNIT_TEST

struct CylinderTest : public ::testing::Test
{
    ThanuvaApp m_app;
    Scene* m_scene = m_app.newScene();
};

TEST_F(CylinderTest, CreationDefault)
{
    CylinderModel* cylinderModel = m_scene->newModelObject<CylinderModel>();
}

#endif // UNIT_TEST

} // namespace Model
