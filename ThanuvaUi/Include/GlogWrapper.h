/*
* ThanuvaUi: UI for Thanuva
*
* Copyright 2016, Saravanan Poosanthiram
* All rights reserved.
*/

#ifndef THANUVAUI_GLOGWRAPPER_H
#define THANUVAUI_GLOGWRAPPER_H

#include <QString>

namespace ThanuvaUi {

extern const char* kAppName;

class GlogWrapper {
public:
    GlogWrapper(const char* argv0);
    ~GlogWrapper();

private:
    const char* kLogInfoExt = ".log.INFO.";
    const char* kLogWarningExt = ".log.WARNING.";
    const char* kLogErrorExt = ".log.ERROR.";
    const int kLogFileSize = 20; // in MB
    const int kLogFilesToKeep = 5;

private:
    QString logdir();
};

}

#endif // THANUVAUI_GLOGWRAPPER_H
