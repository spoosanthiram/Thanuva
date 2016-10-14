/*
 * ThanuvaUi: UI for Thanuva
 *
 * Copyright 2016, Saravanan Poosanthiram
 * All rights reserved.
 */

#include <glog/logging.h>

#include <QApplication>
#include <QDir>

#include "GlogWrapper.h"
#include "MainWindow.h"

namespace ThanuvaUi {

const char* kAppName = "Thanuva";
const char* kVersion = "1.0";
const char* kDomain = "sarvanz.com";

} // namespace ThanuvaUi

int main(int argc, char *argv[])
{
    ThanuvaUi::GlogWrapper logger{argv[0]};
    LOG(INFO) << "Starting " << ThanuvaUi::kAppName << " " << ThanuvaUi::kVersion;

    QApplication app{argc, argv};
    app.setApplicationName(ThanuvaUi::kAppName);
    app.setOrganizationDomain(ThanuvaUi::kDomain);

    ThanuvaUi::MainWindow w{};
    w.show();

    app.exec();

    return 0;
}
