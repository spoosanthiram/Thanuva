/**
* Author: Saravanan Poosanthiram
* $LastChangedBy: ps $
* $LastChangedDate: 2015-03-14 12:57:44 -0400 (Sat, 14 Mar 2015) $
*/

#include "GlViewerException.h"

namespace GlViewer {

const char* GlViewerException::kCreateShader = "glCreateShader() failed for: ";
const char* GlViewerException::kCreateProgram = "glCreateProgram() failed.";
const char* GlViewerException::kCompileShader = "glCompileShader() failed: ";
const char* GlViewerException::kAttachShader = "glAttachShader() failed!";
const char* GlViewerException::kLinkProgram = "glLinkProgram() failed: ";

} // namespace GlViewer
