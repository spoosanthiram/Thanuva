/**
 * Source file for Box and Limiter classes.
 *
 * Author: Saravanan Poosanthiram
 */

#include "Box.h"

#include <boost/property_tree/ptree.hpp>
#include <glog/logging.h>
#ifdef UNIT_TEST
#include <gtest/gtest.h>
#endif

#include "GeometryException.h"
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

Box::Box(const Project& project, const Limiter& limiter)
    : Geometry{project}
    , m_limiter{limiter}
{
    if (!m_limiter.isValid()) {
        GeometryException e{GeometryException::kBoxLimiterInvalid};
        LOG(ERROR) << e.what();
        throw e;
    }
}

void Box::setLimiter(const Limiter& limiter, Core::EmitSignal emitSignal)
{
    if (!limiter.isValid()) {
        GeometryException e{GeometryException::kBoxLimiterInvalid};
        LOG(ERROR) << e.what();
        throw e;
    }

    if (limiter == m_limiter)
        return;

    m_limiter = limiter;

    if (Core::EmitSignal::Emit == emitSignal)
        geometryChanged.emit_signal(); // emit signal
}

void Box::load(const boost::property_tree::ptree& geometryPropTree)
{
    this->loadGeometry(geometryPropTree);

    m_limiter.xlow = geometryPropTree.get<double>(kXLowTag);
    m_limiter.xhigh = geometryPropTree.get<double>(kXHighTag);
    m_limiter.ylow = geometryPropTree.get<double>(kYLowTag);
    m_limiter.yhigh = geometryPropTree.get<double>(kYHighTag);
    m_limiter.zlow = geometryPropTree.get<double>(kZLowTag);
    m_limiter.zhigh = geometryPropTree.get<double>(kZHighTag);
}

void Box::save(boost::property_tree::ptree& geometryPropTree)
{
    this->saveGeometry(geometryPropTree);

    geometryPropTree.put(kXLowTag, m_limiter.xlow);
    geometryPropTree.put(kXHighTag, m_limiter.xhigh);
    geometryPropTree.put(kYLowTag, m_limiter.ylow);
    geometryPropTree.put(kYHighTag, m_limiter.yhigh);
    geometryPropTree.put(kZLowTag, m_limiter.zlow);
    geometryPropTree.put(kZHighTag, m_limiter.zhigh);
}

#ifdef UNIT_TEST

struct BoxTest : public ::testing::Test
{
    Project m_project;
};

TEST_F(BoxTest, CreationDefault)
{
    Box b{m_project};

    Box::Limiter expected{-1.0, 1.0, -1.0, 1.0, -1.0, 1.0};
    EXPECT_EQ(expected, b.limiter());
}

TEST_F(BoxTest, CreationWithLimiter)
{
    Box b{m_project, Box::Limiter{0.1, 10.7, -2.8, 9.8, -20.4, -11.5}};

    Box::Limiter expected{0.1, 10.7, -2.8, 9.8, -20.4, -11.5};
    EXPECT_EQ(expected, b.limiter());
}

#endif // UNIT_TEST

} // namespace Model
