/*
* Model: Model objects for Thanuva
*
* Copyright 2016, Saravanan Poosanthiram
* All rights reserved.
*/

#ifndef MODEL_GLOGWRAPPER_H
#define MODEL_GLOGWRAPPER_H

#include <experimental/filesystem>
#include <set>

namespace fs = std::experimental::filesystem;

namespace Model {

class ThanuvaApp;

class GlogWrapper {
public:
    GlogWrapper(const ThanuvaApp& app);
    ~GlogWrapper();

private:
    fs::path logPath() const;
    void removeOldFiles(std::set<fs::path>& filePaths);

    const ThanuvaApp& m_app;

    fs::path m_infoPathPrefix{"info."};
    fs::path m_warningPathPrefix{"warning."};
    fs::path m_errorPathPrefix{"error."};

    const int kLogFileSize = 20; // in MB
    const int kLogFilesToKeep = 10;
};

}

#endif // MODEL_GLOGWRAPPER_H
