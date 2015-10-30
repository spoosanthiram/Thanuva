/**
* Author: Saravanan Poosanthiram
* $LastChangedBy: ps $
* $LastChangedDate: 2015-03-14 12:57:44 -0400 (Sat, 14 Mar 2015) $
*/

#ifndef GRAPHICS_VERSION_H
#define GRAPHICS_VERSION_H

#include <string>

namespace Graphics {

extern const char* kAppName;
extern const char* kAppBinary;
extern const char* kVersion;
extern const char* kDomain;

std::string version();
std::string internalVersion();

} // namespace Graphics

#endif // GRAPHICS_VERSION_H
