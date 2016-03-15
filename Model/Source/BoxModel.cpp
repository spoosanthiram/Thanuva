/*
 * Model: Model objects for Thanuva
 *
 * Copyright 2016, Saravanan Poosanthiram
 * All rights reserved.
 */

#include "BoxModel.h"

#include <boost/property_tree/ptree.hpp>
#include <glog/logging.h>
#ifdef UNIT_TEST
#include <gtest/gtest.h>
#endif

#include "ModelException.h"
#ifdef UNIT_TEST
#include "Project.h"
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

BoxModel::BoxModel(const Project& project, const Limiter& limiter)
    : ModelObject{project}
    , m_limiter{limiter}
{
    if (!m_limiter.isValid()) {
        ModelException e{ModelException::kInvalidBoxLimiter};
        LOG(ERROR) << e.what();
        throw e;
    }
}

void BoxModel::setLimiter(const Limiter& limiter, Core::EmitSignal emitSignal)
{
    if (!limiter.isValid()) {
        ModelException e{ModelException::kInvalidBoxLimiter};
        LOG(ERROR) << e.what();
        throw e;
    }

    if (limiter == m_limiter)
        return;

    m_limiter = limiter;

    if (Core::EmitSignal::Emit == emitSignal)
        modelObjectChanged.emit_signal(); // emit signal
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
    Project m_project;
};

TEST_F(BoxTest, CreationDefault)
{
    BoxModel b{m_project};

    BoxModel::Limiter expected{-1.0, 1.0, -1.0, 1.0, -1.0, 1.0};
    EXPECT_EQ(expected, b.limiter());
}

TEST_F(BoxTest, CreationWithLimiter)
{
    BoxModel b{m_project, BoxModel::Limiter{0.1, 10.7, -2.8, 9.8, -20.4, -11.5}};

    BoxModel::Limiter expected{0.1, 10.7, -2.8, 9.8, -20.4, -11.5};
    EXPECT_EQ(expected, b.limiter());
}

#endif // UNIT_TEST

} // namespace Model
