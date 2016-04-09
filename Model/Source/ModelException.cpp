/*
 * Model: Model objects for Thanuva
 *
 * Copyright 2016, Saravanan Poosanthiram
 * All rights reserved.
 */

#include "ModelException.h"

namespace Model {

const char* ModelException::kInvalidBoxLimiter = "Limiter for Box is invalid.";
const char* ModelException::kBadExtension =
        "The file path, \"{}\" for the Project is not ending with {}.";

} // namespace Model
