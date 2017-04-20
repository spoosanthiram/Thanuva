/*
 * Model: Model objects for Thanuva
 *
 * Copyright 2016, Saravanan Poosanthiram
 * All rights reserved.
 */

#ifndef Model_ThanuvaApp_h
#define Model_ThanuvaApp_h

#include <filesystem>
#include <memory>
#include <string>

#include <nano_signal_slot.hpp>

#include "GlogWrapper.h"
#include "Scene.h"

namespace Model {

class ThanuvaApp
{
public:
    ThanuvaApp();
    ThanuvaApp(const ThanuvaApp&) = delete;
    ThanuvaApp& operator= (const ThanuvaApp&) = delete;
    ~ThanuvaApp();

    std::string name() const { return m_name; }
    std::string version() const { return m_version; }
    std::string domain() const { return m_domain; }
    fs::path fileExtension() const { return m_fileExtension; }
    fs::path homePath() const;
    fs::path appPath() const;
    fs::path recentDirPath() const { return m_recentDirPath; }
    const std::list<fs::path>& recentScenePaths() const { return m_recentScenePaths; }

    /**
     * Create new scene. The scene object is owned by ThanuvaApp.
     * Creating another scene, opening a scene or closing will make the object go away.
     */
    Scene* newScene();

    /**
     * Open scene from the file. If there is a scene already created/opended, it will be released.
     */
    Scene* openScene(const fs::path& filePath);
    void saveSceneAs(const fs::path& filePath);
    void closeScene();
    void removeFromRecentPaths(const fs::path& filePath) { m_recentScenePaths.remove(filePath); }

private:
    void readSettings();
    void addToRecentPaths(const fs::path& filePath);
    void writeSettings();

    std::string m_name{"Thanuva"};
    std::string m_version{"1.0"};
    std::string m_domain{"sarvanz.com"};
    fs::path m_fileExtension{".tha"};
    mutable fs::path m_appPath{};

    std::unique_ptr<Scene> m_scene{};
    fs::path m_recentDirPath{};

    std::list<fs::path> m_recentScenePaths{};
    int m_nrecentScenePathsToKeep{5};

    GlogWrapper m_glogWrapper;
};

} // namespace Model

#endif // Model_ThanuvaApp_h
