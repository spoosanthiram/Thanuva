/*
 * Model: Model objects for Thanuva
 *
 * Copyright 2016, Saravanan Poosanthiram
 * All rights reserved.
 */

#include "CylinderModel.h"

#include <boost/property_tree/ptree.hpp>
#include <fmt/format.h>
#include <glog/logging.h>
#ifdef UNIT_TEST
#include <gtest/gtest.h>
#endif

#include "ModelException.h"
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

void CylinderModel::loadModel(const boost::property_tree::ptree& modelPropTree)
{
}

void CylinderModel::saveModel(boost::property_tree::ptree& modelPropTree)
{
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

    //CylinderModel::Limiter expected{-1.0, 1.0, -1.0, 1.0, -1.0, 1.0};
    //EXPECT_EQ(expected, boxModel->limiter());
}

#endif // UNIT_TEST

} // namespace Model
