#include <QApplication>
#include <QDir>

#include "MainWindow.h"
#include "Version.h"

#include "glog/logging.h"

namespace Graphics {

const char* kAppName = "GL Viewer";
const char* kAppBinary = "GlViewer";
const char* kVersion = "1.0";
const char* kDomain = "sarvanz.com";

std::string version()
{
    return std::string{kVersion};
}

std::string internalVersion()
{
#ifdef REVISION_NUMBER // comes from cmake definition (-D)
    std::string revisionNumber{std::to_string(REVISION_NUMBER)};
    if (revisionNumber.empty())
        return std::string{kVersion};
    else
        return std::string{kVersion} + "." + revisionNumber;
#else
    return std::string{kVersion};
#endif
}

} // namespace Graphics

namespace GlViewer {

class GLogger {
public:
    GLogger(const char* argv0)
    {
        QString logDirPath = logdir();

        FLAGS_max_log_size = kLogFileSize;

        google::SetLogDestination(google::INFO, (logDirPath + "/" + Graphics::kAppBinary + kLogInfoExt).toUtf8());
        google::SetLogDestination(google::WARNING, (logDirPath + "/" + Graphics::kAppBinary + kLogWarningExt).toUtf8());
        google::SetLogDestination(google::ERROR, (logDirPath + "/" + Graphics::kAppBinary + kLogErrorExt).toUtf8());

        google::InitGoogleLogging(argv0);
    }

    ~GLogger()
    {
        QDir logDir{logdir()};

        QString filterPrefix{Graphics::kAppBinary};

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
        QString execName = QString(Graphics::kAppBinary).toLower();
        QDir logDir{QDir::homePath() + "/" + QString(".%1").arg(execName)};
        if (!logDir.exists()) {
            QDir userHomeDir = QDir::home();
            userHomeDir.mkdir(QString(".%1").arg(execName));
        }
        return logDir.absolutePath();
    }
};

} // namespace GlViewer

int main(int argc, char *argv[])
{
    GlViewer::GLogger logger{argv[0]};
    LOG(INFO) << "Starting " << Graphics::kAppName << " " << Graphics::internalVersion();

    QApplication app{argc, argv};
    app.setApplicationName(Graphics::kAppName);
    app.setOrganizationDomain(Graphics::kDomain);

    GlViewer::MainWindow w{};
    w.show();

    app.exec();

    return 0;
}
