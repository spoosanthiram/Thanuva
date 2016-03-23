/*
 * ThanuvaUi: UI for Thanuva
 *
 * Copyright 2016, Saravanan Poosanthiram
 * All rights reserved.
 */

#include <glog/logging.h>

#include <QApplication>
#include <QDir>

#include "MainWindow.h"

namespace {

const char* kAppName = "Thanuva";
const char* kVersion = "1.0";
const char* kDomain = "sarvanz.com";

class GLogger {
public:
    GLogger(const char* argv0)
    {
        QString logDirPath = logdir();

        FLAGS_max_log_size = kLogFileSize;

        google::SetLogDestination(google::INFO,
                                  (logDirPath + "/" + kAppName + kLogInfoExt).toUtf8());
        google::SetLogDestination(google::WARNING,
                                  (logDirPath + "/" + kAppName + kLogWarningExt).toUtf8());
        google::SetLogDestination(google::ERROR,
                                  (logDirPath + "/" + kAppName + kLogErrorExt).toUtf8());

        google::InitGoogleLogging(argv0);
    }

    ~GLogger()
    {
        QDir logDir{logdir()};

        QString filterPrefix{kAppName};

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

private:
    const char* kLogInfoExt = ".log.INFO.";
    const char* kLogWarningExt = ".log.WARNING.";
    const char* kLogErrorExt = ".log.ERROR.";
    const int kLogFileSize = 20; // in MB
    const int kLogFilesToKeep = 5;

private:
    QString logdir()
    {
        QString name = QString(kAppName).toLower();
        QDir logDir{QDir::homePath() + "/" + QString(".%1").arg(name)};
        if (!logDir.exists()) {
            QDir userHomeDir = QDir::home();
            userHomeDir.mkdir(QString(".%1").arg(name));
        }
        return logDir.absolutePath();
    }
};

} // namespace anonymous

int main(int argc, char *argv[])
{
    GLogger logger{argv[0]};
    LOG(INFO) << "Starting " << kAppName << " " << kVersion;

    QApplication app{argc, argv};
    app.setApplicationName(kAppName);
    app.setOrganizationDomain(kDomain);

    ThanuvaUi::MainWindow w{};
    w.show();

    app.exec();

    return 0;
}
