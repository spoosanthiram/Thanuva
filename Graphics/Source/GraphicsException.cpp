/*
 * Graphics: Graphical objects for Thanuva Model/Geometry obejcts
 *
 * Copyright 2016, Saravanan Poosanthiram
 * All rights reserved.
 */

#include "GraphicsException.h"

namespace Graphics {

const char* GraphicsException::kCreateShader = "glCreateShader() failed for: ";
const char* GraphicsException::kCreateProgram = "glCreateProgram() failed.";
const char* GraphicsException::kCompileShader = "glCompileShader() failed: ";
const char* GraphicsException::kAttachShader = "glAttachShader() failed!";
const char* GraphicsException::kLinkProgram = "glLinkProgram() failed: ";

} // namespace Graphics
