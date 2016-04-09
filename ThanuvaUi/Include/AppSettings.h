/*
 * ThanuvaUi: UI for Thanuva
 *
 * Copyright 2016, Saravanan Poosanthiram
 * All rights reserved.
 */

#ifndef THANUVAUI_APPSETTINGS_H
#define THANUVAUI_APPSETTINGS_H

#include <QRect>
#include <QSettings>

namespace ThanuvaUi {

class AppSettings : public QSettings
{
public:
    static const char* kGeometryLabel;
    static const char* kIsMaximizedLabel;
    static const char* kWindowOpacityLabel;

    static const int kDefaultX = 200;
    static const int kDefaultY = 150;
    static const int kDefaultWidth = 960;
    static const int kDefaulfHeight = 600;
    static const double kDefaultWindowOpacity;

public:
    AppSettings() : QSettings{} {}

    QRect geometry() const
    {
        return this->value(kGeometryLabel, QRect{kDefaultX, kDefaultY, kDefaultWidth, kDefaulfHeight}).toRect();
    }
    bool isMaximized() const { return this->value(kIsMaximizedLabel, false).toBool(); }
    double windowOpacity() const { return this->value(kWindowOpacityLabel, kDefaultWindowOpacity).toDouble(); }

    void setGeometry(const QRect& geometry) { this->setValue(kGeometryLabel, geometry); }
    void setIsMaximized(bool value) { this->setValue(kIsMaximizedLabel, value); }
    void setWindowOpacity(double value) { this->setValue(kWindowOpacityLabel, value); }
};

} // namespace ThanuvaUi

#endif // THANUVAUI_APPSETTINGS_H
