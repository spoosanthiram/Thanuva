/*
 * Core: Common Code used by other modules of Thanuva
 *
 * Copyright 2016, Saravanan Poosanthiram
 * All rights reserved.
 */

#ifndef CORE_COLOR_H
#define CORE_COLOR_H

#include <string>

namespace Core {

/**
 * @brief The Color class abstracts the RGB colors. The values are stored as floats.
 */
class Color
{
public:
    // rgbint_t for representing RGB values as integer.
    // using long so we could potentially move to 16 bit per color components
#ifdef WINDOWS
    using rgbint_t = long long;
#else
    using rgbint_t = long;
#endif

    // constants for 8-bit RGB
    static const int kMaxColorValue = 0xff;
    static const int kBitsPerComponent = 8;

    static const int kIntBase = 16;

public:
    Color()
        : m_r{0.0f}
        , m_g{0.0f}
        , m_b{0.0f}
    {}
    explicit Color(rgbint_t rgb)
    {
        m_b = static_cast<float>(rgb & kMaxColorValue) / kMaxColorValue;
        m_g = static_cast<float>((rgb >> kBitsPerComponent) & kMaxColorValue) / kMaxColorValue;
        m_r = static_cast<float>((rgb >> (2 * kBitsPerComponent)) & kMaxColorValue) /kMaxColorValue;
    }
    Color(int r, int g, int b)
        : m_r{static_cast<float>(r) / kMaxColorValue}
        , m_g{static_cast<float>(g) / kMaxColorValue}
        , m_b{static_cast<float>(b) / kMaxColorValue}
    {}
    Color(float r, float g, float b)
        : m_r{r}
        , m_g{g}
        , m_b{b}
    {}

    float r() const { return m_r; }
    float g() const { return m_g; }
    float b() const { return m_b; }
    int rInt() const { return static_cast<int>(m_r * kMaxColorValue); }
    int gInt() const { return static_cast<int>(m_g * kMaxColorValue); }
    int bInt() const { return static_cast<int>(m_b * kMaxColorValue); }
    /**
     * @return Integer representation of RGB values with R in MSB and G in LSB. Ex: 0x0RRRGGGBBB
     */
    rgbint_t rgb() const;
    /**
     * @return Hexadecimal string of integener that is returned by rgb()
     */
    std::string str() const;

    void setR(float r) { m_r = r; }
    void setG(float g) { m_g = g; }
    void setB(float b) { m_b = b; }
    void set(const std::string& str);

    bool operator == (const Color& c) const
    {
        return (this->rInt() == c.rInt() && this->gInt() == c.rInt() && this->bInt() == c.bInt());
    }
    bool operator != (const Color& c) const { return !(c == *this); }

protected:
    float m_r;
    float m_g;
    float m_b;
};

} // namespace Core

#endif // CORE_COLOR_H
