/*
 * Core: Common Code used by other modules of Thanuva
 *
 * Copyright 2016, Saravanan Poosanthiram
 * All rights reserved.
 */

#include "Color.h"

#include <Algo.h>
#ifdef UNIT_TEST
#include <gtest/gtest.h>
#endif

namespace Core {

Color::rgbint_t Color::rgb() const
{
    int rgb = static_cast<int>(m_r * kMaxColorValue);
    rgb <<= kBitsPerComponent;
    rgb |= static_cast<int>(m_g * kMaxColorValue);
    rgb <<= kBitsPerComponent;
    rgb |= static_cast<int>(m_b * kMaxColorValue);
    return rgb;
}

std::string Color::str() const
{
    return psa::toStringBaseP2(this->rgb(), kIntBase);
}

void Color::set(const std::string& str)
{
    int rgb = std::stoi(str, nullptr, kIntBase);
    m_b = static_cast<float>(rgb & kMaxColorValue) / kMaxColorValue;
    m_g = static_cast<float>((rgb >> kBitsPerComponent) & kMaxColorValue) / kMaxColorValue;
    m_r = static_cast<float>((rgb >> (2 * kBitsPerComponent)) & kMaxColorValue) / kMaxColorValue;
}

#ifdef UNIT_TEST

TEST(ColorTest, Simple)
{
    Color black{};
    EXPECT_EQ(0.0f, black.r());
    EXPECT_EQ(0.0f, black.g());
    EXPECT_EQ(0.0f, black.b());

    Color white{0xffffff};
    EXPECT_EQ(1.0f, white.r());
    EXPECT_EQ(1.0f, white.g());
    EXPECT_EQ(1.0f, white.b());

    Color grey{0.5f, 0.5f, 0.5f};
    EXPECT_EQ(127, grey.rInt());
    EXPECT_EQ(127, grey.gInt());
    EXPECT_EQ(127, grey.bInt());
    EXPECT_EQ(0x7f7f7f, grey.rgb());

    Color c{10, 120, 240};
    EXPECT_EQ(10.0f / 255.0f, c.r());
    EXPECT_EQ(10, c.rInt());
    EXPECT_EQ(120.0f / 255.0f, c.g());
    EXPECT_EQ(120, c.gInt());
    EXPECT_EQ(240.0f / 255.0f, c.b());
    EXPECT_EQ(240, c.bInt());
    EXPECT_EQ(0x0a78f0, c.rgb());
    std::string strExpected{"a78f0"};
    EXPECT_EQ(strExpected, c.str());

    Color c2{0.3f, 0.7f, 0.05f};
    EXPECT_EQ(static_cast<int>(0.3f * 255), c2.rInt());
    EXPECT_EQ(static_cast<int>(0.7f * 255), c2.gInt());
    EXPECT_EQ(static_cast<int>(0.05f * 255), c2.bInt());
}

#endif // UNIT_TEST

} // namespace Core
