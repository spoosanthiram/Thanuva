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

class StartUpPage : public QWidget, private Ui::StartUpPage
{
    Q_OBJECT

public:
    StartUpPage(QWidget* parent);

signals:
    void newThanuvaProject();

protected:
    void resizeEvent(QResizeEvent* event) override;
};

} // namespace ThanuvaUi

#endif // THANUVAUI_STARTUPPAGE_H
