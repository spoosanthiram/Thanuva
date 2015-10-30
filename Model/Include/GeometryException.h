/**
 * Author: Saravanan Poosanthiram
 * $LastChangedBy: ps $
 * $LastChangedDate: 2015-03-14 12:57:44 -0400 (Sat, 14 Mar 2015) $
 */

#ifndef MODEL_GEOMETRYEXCEPTION_H
#define MODEL_GEOMETRYEXCEPTION_H

#include "GraphicsException.h"

namespace Model {

class GeometryException : public Core::GraphicsException {
public:
    static const char* kBoxLimiterInvalid;

public:
    GeometryException(const char* message)
        : Core::GraphicsException{message}
    {}
    GeometryException(const std::string& message)
        : Core::GraphicsException{message}
    {}
};

} // namespace Model

#endif // MODEL_GEOMETRYEXCEPTION_H
