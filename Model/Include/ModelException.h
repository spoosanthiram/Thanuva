/*
 * Model: Model objects for Thanuva
 *
 * Copyright 2016, Saravanan Poosanthiram
 * All rights reserved.
 */

#ifndef MODEL_MODELEXCEPTION_H
#define MODEL_MODELEXCEPTION_H

#include "GraphicsException.h"

namespace Model {

class ModelException : public Core::GraphicsException {
public:
    static const char* kInvalidBoxLimiter;
    static const char* kBadExtension;

public:
    ModelException(const char* message)
        : Core::GraphicsException{message}
    {}
    ModelException(std::string&& message)
        : Core::GraphicsException{std::move(message)}
    {}
};

} // namespace Model

#endif // MODEL_MODELEXCEPTION_H
