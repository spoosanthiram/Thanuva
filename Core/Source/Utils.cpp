/*
 * Core: Common Code used by other modules of Thanuva
 *
 * Copyright 2016, Saravanan Poosanthiram
 * All rights reserved.
 */

#include "Utils.h"

#include <algorithm>
#include <cctype>
#include <locale>

namespace Core {

std::string toLower(std::string str)
{
    std::locale defaultLocale{};
    for (auto& ch : str)
        ch = std::tolower(ch, defaultLocale);
    return str;
}

std::string trim(std::string str)
{
    auto isNotSpace = [](auto& ch) { return !std::isspace(ch); };
    str.erase(std::find_if(str.rbegin(), str.rend(), isNotSpace).base(), str.end());
    str.erase(str.begin(), std::find_if(str.begin(), str.end(), isNotSpace));
    return str;
}

void skipWhitespace(std::ifstream& textStream)
{
    while (textStream && std::isspace(textStream.get()))
        ;
    textStream.unget();
}

} // namespace Core
