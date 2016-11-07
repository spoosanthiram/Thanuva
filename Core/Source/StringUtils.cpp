/*
 * Core: Common Code used by other modules of Thanuva
 *
 * Copyright 2016, Saravanan Poosanthiram
 * All rights reserved.
 */

#include "StringUtils.h"

#include <locale>

namespace Core {

std::string toLower(const std::string& str)
{
    std::string result{};
    for (auto ch : str)
        result.push_back(std::tolower(ch, std::locale{}));
    return result;
}

} // namespace Core
