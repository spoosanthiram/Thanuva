/*
 * ThanuvaUi: UI for Thanuva
 *
 * Copyright 2016, Saravanan Poosanthiram
 * All rights reserved.
 */

#ifndef THANUVAUI_STARTUPPAGE_H
#define THANUVAUI_STARTUPPAGE_H

#include "ui_StartUpPage.h"

namespace ThanuvaUi {

class MainWindow;

class StartUpPage : public QWidget, private Ui::StartUpPage
{
    Q_OBJECT

public:
    StartUpPage(ThanuvaUi::MainWindow* mainWindow);

signals:
    void newThanuvaProject();

private:
    void resizeEvent(QResizeEvent* event) override;

    ThanuvaUi::MainWindow* m_mainWindow;
};

} // namespace ThanuvaUi

#endif // THANUVAUI_STARTUPPAGE_H
