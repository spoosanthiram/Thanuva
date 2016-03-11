/*
 * Core: Common Code used by other modules of Thanuva
 *
 * Copyright 2016, Saravanan Poosanthiram
 * All rights reserved.
 */

#include "Material.h"

#include <regex>

#include <cppformat/format.h>
#ifdef UNIT_TEST
#include <gtest/gtest.h>
#endif

#include "CoreDef.h"
#include "GraphicsException.h"

namespace {

const char* kDiffuseColorPrefix = "dc:";
const char* kSpecularColorPrefix = "sc:";
const char* kShininessPrefix = "s:";

} // anonymous namespace

namespace Core {

Material Material::defaultMaterial()
{
    return Material{Core::Color{180, 90, 4}, Core::Color{90, 45, 2}, 128.0f};
}

std::string Material::str() const
{
    std::string result;

    result.append(kDiffuseColorPrefix);
    result.append(m_diffuseColor.str());
    result.append(" ");

    result.append(kSpecularColorPrefix);
    result.append(m_specularColor.str());
    result.append(" ");

    result.append(kShininessPrefix);
    result.append(std::to_string(m_shininess));

    return result;
}

void Material::set(const std::string& str)
{
    std::regex re{kWhitespaceRegEx};

    auto it = std::sregex_token_iterator(str.begin(), str.end(), re, -1);
    for (; it != std::sregex_token_iterator(); ++it) {
        auto cstr = (*it).str();
        if (cstr.size() > 3 && 'd' == cstr[0] && 'c' == cstr[1] && ':' == cstr[2]) // diffuse color
            m_diffuseColor.set(cstr.substr(3));
        else if (cstr.size() > 3 && 's' == cstr[0] && 'c' == cstr[1] && ':' == cstr[2]) // specular color
            m_specularColor.set(cstr.substr(3));
        else if (cstr.size() > 2 && 's' == cstr[0] && ':' == cstr[1]) // shininess
            m_shininess = std::stof(cstr.substr(2));
        else
            throw GraphicsException{fmt::format(GraphicsException::kBadMaterialString, str)};
    }
}

#ifdef UNIT_TEST

TEST(MaterialTest, Simple)
{
    Material mat{Color{127, 0, 0}, Color{34, 54, 234}, 64.0};
    EXPECT_EQ("dc:7f0000 sc:2236ea s:64.000000", mat.str());

    EXPECT_THROW(mat.set("as46234"), GraphicsException);

    mat.set("dc:b45a04 sc:5a2d02 s:128.000000");
    EXPECT_EQ(180.0f / Color::kMaxColorValue, mat.diffuseColor().r());
    EXPECT_EQ(0x5a, mat.diffuseColor().gInt());
    EXPECT_EQ(0x5a2d02, mat.specularColor().rgb());
    EXPECT_EQ(128.0f, mat.shininess());
}

#endif // UNIT_TEST

} // namespace Core
