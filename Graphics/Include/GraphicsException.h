/*
 * Graphics: Graphical objects for Thanuva Model/Geometry obejcts
 *
 * Copyright 2016, Saravanan Poosanthiram
 * All rights reserved.
 */

#ifndef GRAPHICS_GRAPHICSEXCEPTION_H
#define GRAPHICS_GRAPHICSEXCEPTION_H

#include "ThanuvaException.h"

namespace Graphics {

class GraphicsException : public Core::ThanuvaException
{
public:
    static const char* kCreateShader;
    static const char* kCreateProgram;
    static const char* kCompileShader;
    static const char* kAttachShader;
    static const char* kLinkProgram;

public:
    GraphicsException(const char* message)
        : Core::ThanuvaException{message}
    {}
    GraphicsException(std::string&& message)
        : Core::ThanuvaException{std::move(message)}
    {}
};

} // namespace Graphics

#endif // GRAPHICS_GRAPHICSEXCEPTION_H
