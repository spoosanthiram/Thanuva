/*
 * Model: Model objects for Thanuva
 *
 * Copyright 2016, Saravanan Poosanthiram
 * All rights reserved.
 */

#include "ModelException.h"

namespace Model {

// General
const char* ModelException::kPathNotFound = "The file, {} could not be opened.";

// ModelObject messages
const char* ModelException::kEmptyName = "Name cannot be empty.";
const char* ModelException::kInvalidType = "Invalid ModelObjec type.";
const char* ModelException::kInvalidBoxLimiter = "Invalid Box limiters.";

// Scene messages
const char* ModelException::kFailedToRead = "Could not read the scene file: {}.";
const char* ModelException::kFailedToWrite = "Could not write to the scene file: {}.";
const char* ModelException::kInvalidPath = "File Path {} is invalid.";

} // namespace Model
