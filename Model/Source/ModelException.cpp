/*
 * Model: Model objects for Thanuva
 *
 * Copyright 2016, Saravanan Poosanthiram
 * All rights reserved.
 */

#include "ModelException.h"

namespace Model {

const char* ModelException::kEmptyName = "Name cannot be empty.";

const char* ModelException::kInvalidBoxLimiter = "Limiter provided {} for Box is invalid.";

const char* ModelException::kPathNotFound = "The Scene file, {} could not be opened.";
const char* ModelException::kFailedToRead = "Could not read the scene file: {}.";
const char* ModelException::kFailedToWrite = "Could not write to the scene file: {}.";
const char* ModelException::kInvalidPath = "File Path {} is invalid.";

} // namespace Model
