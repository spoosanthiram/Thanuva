/*
 * ThanuvaUi: UI for Thanuva
 *
 * Copyright 2016, Saravanan Poosanthiram
 * All rights reserved.
 */

#include "StartUpPage.h"

#include <QResizeEvent>

#include "MainWindow.h"
#include "ThanuvaApp.h"

namespace ThanuvaUi {

StartUpPage::StartUpPage(ThanuvaUi::MainWindow* mainWindow)
    : QWidget{mainWindow}
    , m_mainWindow{mainWindow}
{
    this->setupUi(this);

    const std::list<fs::path>& recentPaths = m_mainWindow->app().recentScenePaths();
    for (auto& filePath : recentPaths)
        m_scenesVerticalLayout->addWidget(new QLabel(filePath.string().c_str(), this));

    connect(m_newSceneButton, &QPushButton::clicked, this, &StartUpPage::newThanuvaProject);
}

void StartUpPage::resizeEvent(QResizeEvent* event)
{
    m_topMarginVSpacer->changeSize(20, event->size().width() * 0.1, QSizePolicy::Minimum, QSizePolicy::Fixed);
    m_leftMarginHSpacer->changeSize(event->size().width() * 0.2, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);
}

} // namespace ThanuvaUi
