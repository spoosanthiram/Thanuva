/**
 * Source file for Material class.
 *
 * Author: Saravanan Poosanthiram
 */

#include "Material.h"

#include <regex>
#ifdef UNIT_TEST
#include <gtest/gtest.h>
#endif

#include "CoreDef.h"

namespace {

const char* kDiffuseColorPrefix = "dc:";
const char* kSpecularColorPrefix = "sc:";
const char* kShininessPrefix = "s:";

} // anonymous

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
        auto colorStr = (*it).str();
        if ('d' == colorStr[0] && 'c' == colorStr[1] && ':' == colorStr[2]) // diffuse color
            m_diffuseColor.set(colorStr.substr(3));
        else if ('s' == colorStr[0] && 'c' == colorStr[1] && ':' == colorStr[2]) // specular color
            m_specularColor.set(colorStr.substr(3));
        else if ('s' == colorStr[0] && ':' == colorStr[1]) // shininess
            m_shininess = std::stof(colorStr.substr(2));
    }
}

#ifdef UNIT_TEST

TEST(MaterialTest, Simple)
{
}

#endif // UNIT_TEST

} // namespace Core
