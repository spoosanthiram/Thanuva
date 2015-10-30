/**
* Author: Saravanan Poosanthiram
* $LastChangedBy: ps $
* $LastChangedDate: 2015-03-14 12:57:44 -0400 (Sat, 14 Mar 2015) $
*/

#ifndef GLVIEWER_GLVIEWEREXCEPTION_H
#define GLVIEWER_GLVIEWEREXCEPTION_H

#include "GraphicsException.h"

namespace GlViewer {

class GlViewerException : public Core::GraphicsException {
public:
    static const char* kCreateShader;
    static const char* kCreateProgram;
    static const char* kCompileShader;
    static const char* kAttachShader;
    static const char* kLinkProgram;

public:
    GlViewerException(const char* message)
        : Core::GraphicsException{message}
    {}
    GlViewerException(const std::string& message)
        : Core::GraphicsException{message}
    {}
};

} // namespace GlViewer

#endif // GLVIEWER_GLVIEWEREXCEPTION_H
