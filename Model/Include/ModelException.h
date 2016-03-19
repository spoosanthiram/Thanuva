/*
 * Model: Model objects for Thanuva
 *
 * Copyright 2016, Saravanan Poosanthiram
 * All rights reserved.
 */

#ifndef MODEL_MODELEXCEPTION_H
#define MODEL_MODELEXCEPTION_H

#include "ThanuvaException.h"

namespace Model {

class ModelException : public Core::ThanuvaException
{
public:
    static const char* kInvalidBoxLimiter;
    static const char* kBadExtension;

public:
    ModelException(const char* message)
        : Core::ThanuvaException{message}
    {}
    ModelException(std::string&& message)
        : Core::ThanuvaException{std::move(message)}
    {}
};

} // namespace Model

#endif // MODEL_MODELEXCEPTION_H
