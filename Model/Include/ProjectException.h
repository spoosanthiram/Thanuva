/**
 * Author: Saravanan Poosanthiram
 * $LastChangedBy: ps $
 * $LastChangedDate: 2015-03-17 19:17:00 -0400 (Tue, 17 Mar 2015) $
 */

#ifndef MODEL_PROJECTEXCEPTION_H
#define MODEL_PROJECTEXCEPTION_H

#include "GraphicsException.h"

namespace Model {

class ProjectException : public Core::GraphicsException {
public:
    static const char* kBadExtension;

public:
    ProjectException(const char* message)
        : Core::GraphicsException{message}
    {}
    ProjectException(const std::string& message)
        : Core::GraphicsException{message}
    {}
};

} // namespace Model

#endif // MODEL_PROJECTEXCEPTION_H
