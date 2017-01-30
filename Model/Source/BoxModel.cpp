/*
 * Model: Model objects for Thanuva
 *
 * Copyright 2016, Saravanan Poosanthiram
 * All rights reserved.
 */

#include "BoxModel.h"

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

const char* kXLowTag = "xlow";
const char* kXHighTag = "xhigh";
const char* kYLowTag = "ylow";
const char* kYHighTag = "yhigh";
const char* kZLowTag = "zlow";
const char* kZHighTag = "zhigh";

} // anonymous

namespace Model {

std::string BoxModel::Limiter::str() const
{
    return fmt::format("[{}, {}]; [{}, {}]; [{}, {}]", xlow, xhigh, ylow, yhigh, zlow, zhigh);
}

BoxModel::BoxModel(const Scene& scene, const Limiter& limiter)
    : ModelObject{scene}
    , m_limiter{limiter}
{
    this->setLimiter(m_limiter); // to get it validated

    limiterChanged.connect<ModelObject, &ModelObject::emitModelObjectChanged>(this);
}

void BoxModel::setLimiter(const Limiter& limiter)
{
    if (!limiter.isValid()) {
        ModelException e{fmt::format(ModelException::kInvalidBoxLimiter, limiter.str())};
        LOG(ERROR) << e.what();
        throw e;
    }

    if (limiter == m_limiter)
        return;

    m_limiter = limiter;
    limiterChanged.emit_signal(); // emit signal
}

void BoxModel::loadModel(const boost::property_tree::ptree& modelPropTree)
{
    m_limiter.xlow = modelPropTree.get<double>(kXLowTag);
    m_limiter.xhigh = modelPropTree.get<double>(kXHighTag);
    m_limiter.ylow = modelPropTree.get<double>(kYLowTag);
    m_limiter.yhigh = modelPropTree.get<double>(kYHighTag);
    m_limiter.zlow = modelPropTree.get<double>(kZLowTag);
    m_limiter.zhigh = modelPropTree.get<double>(kZHighTag);
}

void BoxModel::saveModel(boost::property_tree::ptree& modelPropTree)
{
    modelPropTree.put(kXLowTag, m_limiter.xlow);
    modelPropTree.put(kXHighTag, m_limiter.xhigh);
    modelPropTree.put(kYLowTag, m_limiter.ylow);
    modelPropTree.put(kYHighTag, m_limiter.yhigh);
    modelPropTree.put(kZLowTag, m_limiter.zlow);
    modelPropTree.put(kZHighTag, m_limiter.zhigh);
}

#ifdef UNIT_TEST

struct BoxTest : public ::testing::Test
{
    ThanuvaApp m_app;
    Scene* m_scene = m_app.newScene();
};

TEST_F(BoxTest, CreationDefault)
{
    BoxModel* boxModel = m_scene->newModelObject<BoxModel>();

    BoxModel::Limiter expected{-1.0, 1.0, -1.0, 1.0, -1.0, 1.0};
    EXPECT_EQ(expected, boxModel->limiter());
}

TEST_F(BoxTest, CreationWithLimiter)
{
    BoxModel* boxModel = m_scene->newModelObject<BoxModel>(BoxModel::Limiter{0.1, 10.7, -2.8, 9.8, -20.4, -11.5});

    BoxModel::Limiter expected{0.1, 10.7, -2.8, 9.8, -20.4, -11.5};
    EXPECT_EQ(expected, boxModel->limiter());
}

#endif // UNIT_TEST

} // namespace Model
