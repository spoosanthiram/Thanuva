/*
 * Model: Model objects for Thanuva
 *
 * Copyright 2016, Saravanan Poosanthiram
 * All rights reserved.
 */

#include "ThanuvaApp.h"

#include <cstdlib>

#include <fmt/format.h>
#include <glog/logging.h>
#ifdef UNIT_TEST
#include <gtest/gtest.h>
#endif

#ifdef UNIT_TEST
#include "BoxModel.h"
#endif
#include "ModelException.h"
#include "Utils.h"

namespace Model {

ThanuvaApp::ThanuvaApp()
    : m_recentDirPath{fs::current_path()}
    , m_glogWrapper{*this}
{
    LOG(INFO) << fmt::format("Starting App {} {}", this->name(), this->version());
}

fs::path ThanuvaApp::homePath() const
{
#ifdef WIN32
    return fs::path{std::string{std::getenv("HOMEDRIVE")} + std::getenv("HOMEPATH")};
#else
    return fs::path{std::string{std::getenv("HOME")}};
#endif
}

fs::path ThanuvaApp::appPath() const
{
    if (m_appPath.empty()) {
        m_appPath = this->homePath();
        m_appPath /= fs::path{std::string{"."} + Core::toLower(m_name)};
    }
    return m_appPath;
}

Scene* ThanuvaApp::newScene()
{
    m_scene = std::make_unique<Scene>(*this);
    return m_scene.get();
}

Scene* ThanuvaApp::openScene(const fs::path& filePath)
{
    fs::path pathToOpen = filePath;
    if (pathToOpen.is_relative())
        pathToOpen = fs::current_path() / pathToOpen;

    if (!fs::exists(pathToOpen)) {
        ModelException e{fmt::format(ModelException::kPathNotFound, filePath.string())};
        LOG(ERROR) << e.what();
        throw e;
    }

    m_scene = std::make_unique<Scene>(*this, filePath);
    this->addToRecentPaths(filePath);
    return m_scene.get();
}

void ThanuvaApp::saveScene()
{
    CHECK(!m_scene->filePath().empty()) << "Saving without file path!";
    m_scene->write();
}

void ThanuvaApp::saveSceneAs(const fs::path& filePath)
{
    fs::path pathToSave = filePath;
    if (pathToSave.is_relative())
        pathToSave = fs::current_path() / pathToSave;

    if (filePath.empty() || !fs::exists(pathToSave.parent_path()))
        throw ModelException{fmt::format(ModelException::kInvalidPath, filePath.string())};

    m_scene->setFilePath(pathToSave);
    m_scene->write();
    this->addToRecentPaths(filePath);
}

void ThanuvaApp::closeScene()
{
    m_scene.release();
}

void ThanuvaApp::addToRecentPaths(const fs::path& filePath)
{
    m_recentDirPath = filePath.parent_path();

    m_recentScenePaths.remove(filePath);
    m_recentScenePaths.push_front(filePath);
    if (m_recentScenePaths.size() > m_nrecentScenePathsToKeep)
        m_recentScenePaths.pop_back();
}

#ifdef UNIT_TEST

TEST(ThanuvaAppTest, Simple)
{
    ThanuvaApp app;
    Scene* scene = app.newScene();
    BoxModel* boxModel = scene->newModelObject<BoxModel>();
    boxModel->setLimiter(BoxModel::Limiter{0.0, 2.0, 0.0, 1.0, -1.0, 1.0});
    app.saveSceneAs("SimpleTest.tha");
}

#endif // UNIT_TEST

} // namespace Model
