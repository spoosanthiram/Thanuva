/*
 * Core: Common Code used by other modules of Thanuva
 *
 * Copyright 2016, Saravanan Poosanthiram
 * All rights reserved.
 */

#ifndef CORE_GRAPHICSEXCEPTION_H
#define CORE_GRAPHICSEXCEPTION_H

#include <exception>
#include <string>

namespace Core {

class GraphicsException : public std::exception {
public:
    static const char* kBadMaterialString;

public:
    GraphicsException(const char* message)
        : std::exception{}
        , m_message{message}
    {}
    GraphicsException(std::string&& message)
        : std::exception{}
        , m_message{message}
    {}

    const char* what() const noexcept override { return m_message.c_str(); }

private:
    std::string m_message;
};

} // namespace Core

#endif // CORE_GRAPHICSEXCEPTION_H
