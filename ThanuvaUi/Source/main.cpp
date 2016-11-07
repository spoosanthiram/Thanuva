/*
 * ThanuvaUi: UI for Thanuva
 *
 * Copyright 2016, Saravanan Poosanthiram
 * All rights reserved.
 */

#include <QApplication>
#include <QDir>

#include "GlogWrapper.h"
#include "MainWindow.h"
#include "ThanuvaApp.h"

int main(int argc, char *argv[])
{
    Model::ThanuvaApp app;

    QApplication qtApp{argc, argv};
    qtApp.setApplicationName(app.name().c_str());
    qtApp.setOrganizationDomain(app.domain().c_str());

    ThanuvaUi::MainWindow w{app};
    w.show();

    qtApp.exec();

    return 0;
}
