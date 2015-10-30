/**
 * Author: Saravanan Poosanthiram
 * $LastChangedBy: ps $
 * $LastChangedDate: 2015-03-14 12:57:44 -0400 (Sat, 14 Mar 2015) $
 */

#ifndef CORE_GRAPHICSEXCEPTION_H
#define CORE_GRAPHICSEXCEPTION_H

#include <exception>
#include <string>

#ifdef _MSC_VER
#define noexcept
#endif

namespace Core {

class GraphicsException : public std::exception {
public:
    GraphicsException(const char* message)
        : std::exception{}
#ifndef _MSC_VER
        , m_message{message}
#else
        , m_message(message)
#endif
    {}
    GraphicsException(const std::string& message)
        : std::exception{}
#ifndef _MSC_VER
        , m_message{message}
#else
        , m_message(message)
#endif
    {}

    const char* what() const noexcept override { return m_message.c_str(); }

private:
    std::string m_message;
};

} // namespace Core

#endif // CORE_GRAPHICSEXCEPTION_H
