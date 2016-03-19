/*
 * Core: Common Code used by other modules of Thanuva
 *
 * Copyright 2016, Saravanan Poosanthiram
 * All rights reserved.
 */

#ifndef CORE_MATERIAL_H
#define CORE_MATERIAL_H

#include "Color.h"

namespace Core {

/**
 * @brief The Material class
 */
class Material
{
public:
    static Material defaultMaterial();

public:
    Material()
        : m_diffuseColor{}
        , m_specularColor{}
        , m_shininess{0.0f}
    {}
    Material(const Color& diffuseColor, const Color& specularColor, float shininess)
        : m_diffuseColor{diffuseColor}
        , m_specularColor{specularColor}
        , m_shininess{shininess}
    {}

    const Color diffuseColor() const { return m_diffuseColor; }
    const Color specularColor() const { return m_specularColor; }
    float shininess() const { return m_shininess; }
    std::string str() const;

    void setDiffuseColor(const Core::Color& diffuseColor) { m_diffuseColor = diffuseColor; }
    void setSpecularColor(const Core::Color& specularColor) { m_specularColor = specularColor; }
    void set(const std::string& str);

    bool operator==(const Material& rhs) const
    {
        return rhs.m_diffuseColor == m_diffuseColor
                && rhs.m_specularColor == m_specularColor
                && rhs.m_shininess == m_shininess;
    }

private:
    Color m_diffuseColor;
    Color m_specularColor;
    float m_shininess;
};

} // namespace Core

#endif // CORE_MATERIAL_H
