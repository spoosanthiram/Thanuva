/*
* ThanuvaUi: UI for Thanuva
*
* Copyright 2016, Saravanan Poosanthiram
* All rights reserved.
*/

#include "GlogWrapper.h"

#include <glog/logging.h>
#include <QDir>

namespace ThanuvaUi {

GlogWrapper::GlogWrapper(const char* argv0)
{
    QString logDirPath = logdir();

    FLAGS_max_log_size = kLogFileSize;

    google::SetLogDestination(google::GLOG_INFO,
        (logDirPath + "/" + kAppName + kLogInfoExt).toUtf8());
    google::SetLogDestination(google::GLOG_WARNING,
        (logDirPath + "/" + kAppName + kLogWarningExt).toUtf8());
    google::SetLogDestination(google::GLOG_ERROR,
        (logDirPath + "/" + kAppName + kLogErrorExt).toUtf8());

    google::InitGoogleLogging(argv0);
}

GlogWrapper::~GlogWrapper()
{
    QDir logDir{ logdir() };

    QString filterPrefix{ kAppName };

    QStringList logFileFilters;
    logFileFilters << filterPrefix + kLogInfoExt + "*";    // for INFO log files
    logFileFilters << filterPrefix + kLogWarningExt + "*"; // for WARNING log files
    logFileFilters << filterPrefix + kLogErrorExt + "*";   // for ERROR log files

    for (auto& filter : logFileFilters) {
        logDir.setNameFilters(QStringList(filter));
        QStringList files = logDir.entryList();
        files.sort();
        if (files.size() > kLogFilesToKeep) {
            int nFilesToRemove = files.size() - kLogFilesToKeep;
            for (int i = 0; i < nFilesToRemove; ++i)
                logDir.remove(files[i]);
        }
    }
}

QString GlogWrapper::logdir()
{
    QString name = QString(kAppName).toLower();
    QDir logDir{ QDir::homePath() + "/" + QString(".%1").arg(name) };
    if (!logDir.exists()) {
        QDir userHomeDir = QDir::home();
        userHomeDir.mkdir(QString(".%1").arg(name));
    }
    return logDir.absolutePath();
}

}
