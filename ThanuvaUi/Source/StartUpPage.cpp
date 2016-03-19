/*
 * ThanuvaUi: UI for Thanuva
 *
 * Copyright 2016, Saravanan Poosanthiram
 * All rights reserved.
 */

#include "StartUpPage.h"

#include <QResizeEvent>

namespace GlViewer {

StartUpPage::StartUpPage(QWidget* parent)
    : QWidget{parent}
{
    this->setupUi(this);
}

void StartUpPage::resizeEvent(QResizeEvent* event)
{
    m_topMarginVSpacer->changeSize(20, event->size().width() * 0.1, QSizePolicy::Minimum, QSizePolicy::Fixed);
    m_leftMarginHSpacer->changeSize(event->size().width() * 0.2, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);
}

} // namespace GlViewer
