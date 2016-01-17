/**
 * Color class header file.
 *
 * Author: Saravanan Poosanthiram
 */

#ifndef CORE_COLOR_H
#define CORE_COLOR_H

#include <string>

namespace Core {

class Color {
public:
    static const int kMaxColorValue = 0xff;
    static const int kBitsPerComponent = 8;

    static const int kIntBase = 16;

public:
    Color()
        : m_r{0.0f}
        , m_g{0.0f}
        , m_b{0.0f}
    {}
#ifdef WINDOWS
    explicit Color(long long rgb)
#else
    explicit Color(long rgb)
#endif
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
#ifdef WINDOWS
    long long rgb() const;
#else
    long rgb() const;
#endif
    std::string str() const;

    void setR(float r) { m_r = r; }
    void setG(float g) { m_g = g; }
    void setB(float b) { m_b = b; }
    void set(const std::string& str);

    bool operator == (const Color& c) const { return (c.m_r == m_r && c.m_g == m_g && c.m_b == m_b); }
    bool operator != (const Color& c) const { return !(c == *this); }

protected:
    float m_r;
    float m_g;
    float m_b;
};

} // namespace Core

#endif // CORE_COLOR_H
