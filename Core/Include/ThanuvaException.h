/*
 * Core: Common Code used by other modules of Thanuva
 *
 * Copyright 2016, Saravanan Poosanthiram
 * All rights reserved.
 */

#ifndef CORE_THANUVAEXCEPTION_H
#define CORE_THANUVAEXCEPTION_H

#include <exception>
#include <string>

namespace Core {

class ThanuvaException : public std::exception
{
public:
    static const char* kBadMaterialString;

public:
    ThanuvaException(const char* message)
        : std::exception{}
        , m_message{message}
    {}
    ThanuvaException(std::string&& message)
        : std::exception{}
        , m_message{message}
    {}

    const char* what() const noexcept override { return m_message.c_str(); }

private:
    std::string m_message;
};

} // namespace Core

#endif // CORE_THANUVAEXCEPTION_H
