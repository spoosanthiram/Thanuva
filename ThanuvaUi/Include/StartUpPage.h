/*
 * ThanuvaUi: UI for Thanuva
 *
 * Copyright 2016, Saravanan Poosanthiram
 * All rights reserved.
 */

#ifndef GLVIEWER_STARTUPPAGE_H
#define GLVIEWER_STARTUPPAGE_H

#include "ui_StartUpPage.h"

namespace GlViewer {

class StartUpPage : public QWidget, private Ui::StartUpPage
{
public:
    StartUpPage(QWidget* parent);

protected:
    void resizeEvent(QResizeEvent* event) override;
};

} // namespace GlViewer

#endif // GLVIEWER_STARTUPPAGE_H
