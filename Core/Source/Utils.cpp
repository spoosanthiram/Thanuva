/*
 * Core: Common Code used by other modules of Thanuva
 *
 * Copyright 2016, Saravanan Poosanthiram
 * All rights reserved.
 */

#include "Utils.h"

#include <cctype>
#include <locale>

namespace Core {

std::string toLower(const std::string& str)
{
    std::string result{};
    std::locale defaultLocale{};
    for (auto ch : str)
        result.push_back(std::tolower(ch, defaultLocale));
    return result;
}

void skipWhitespace(std::ifstream& textStream)
{
    while (textStream && std::isspace(textStream.get()))
        ;
    textStream.unget();
}

} // namespace Core
