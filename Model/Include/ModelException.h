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

    static const char* kPathNotFound;
    static const char* kFailedToRead;
    static const char* kFailedToWrite;
    static const char* kInvalidPath;

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
