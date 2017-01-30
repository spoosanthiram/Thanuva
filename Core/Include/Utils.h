/*
 * Core: Common Code used by other modules of Thanuva
 *
 * Copyright 2016, Saravanan Poosanthiram
 * All rights reserved.
 */

#ifndef CORE_UTILS_H
#define CORE_UTILS_H

#include <fstream>
#include <string>

namespace Core {

std::string toLower(std::string str);
std::string trim(std::string str);

void skipWhitespace(std::ifstream& textStream);

} // namespace Core

#endif // CORE_UTILS_H
