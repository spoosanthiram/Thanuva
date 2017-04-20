/*
* Model: Model objects for Thanuva
*
* Copyright 2016, Saravanan Poosanthiram
* All rights reserved.
*/

#include "GlogWrapper.h"

#include <cstdlib>
#include <filesystem>

#include <glog/logging.h>

#include "ThanuvaApp.h"
#include "Utils.h"

namespace Model {

GlogWrapper::GlogWrapper(const ThanuvaApp& app)
    : m_app{app}
{
    FLAGS_max_log_size = kLogFileSize;

    fs::path logPath{this->logPath()};

    fs::path infoBasePath{logPath / m_infoPathPrefix};
    google::SetLogDestination(google::GLOG_INFO, infoBasePath.string().c_str());
    fs::path warningBasePath{logPath / m_warningPathPrefix};
    google::SetLogDestination(google::GLOG_WARNING, warningBasePath.string().c_str());
    fs::path errorBasePath{logPath / m_errorPathPrefix};
    google::SetLogDestination(google::GLOG_ERROR, errorBasePath.string().c_str());

    google::InitGoogleLogging(Core::toLower(m_app.name()).c_str());
}

GlogWrapper::~GlogWrapper()
{
    std::set<fs::path> infoPaths;
    std::set<fs::path> warningPaths;
    std::set<fs::path> errorPaths;

    for (fs::directory_entry entry : fs::directory_iterator{this->logPath()}) {
        const std::string& filePathStr = entry.path().string();
        if (filePathStr.find(m_infoPathPrefix.string()) != std::string::npos) // INFO log file
            infoPaths.insert(entry.path());
        else if (filePathStr.find(m_warningPathPrefix.string()) != std::string::npos) // WARNING log file
            warningPaths.insert(entry.path());
        else if (filePathStr.find(m_errorPathPrefix.string()) != std::string::npos) // ERROR log file
            errorPaths.insert(entry.path());
    }

    this->removeOldFiles(infoPaths);
    this->removeOldFiles(warningPaths);
    this->removeOldFiles(errorPaths);

    google::ShutdownGoogleLogging();
}

fs::path GlogWrapper::logPath() const
{
    fs::path logPathPart{"log"};
    fs::path dirPath = m_app.appPath() / logPathPart;
    if (!fs::exists(dirPath))
        fs::create_directory(dirPath);
    return dirPath;
}

void GlogWrapper::removeOldFiles(std::set<fs::path>& filePaths)
{
    while (filePaths.size() > kLogFilesToKeep) {
        auto it = filePaths.begin();
        try {
            fs::remove(*it);
        }
        catch (...) {}
        filePaths.erase(it);
    }
}

}
