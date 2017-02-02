/*
 * ThanuvaUi: UI for Thanuva
 *
 * Copyright 2016, Saravanan Poosanthiram
 * All rights reserved.
 */

#ifndef THANUVAUI_CYLINDERDIALOG_H
#define THANUVAUI_CYLINDERDIALOG_H

#include "GeometryDialog.h"
#include "ui_CylinderWidget.h"

namespace Model { class CylinderModel; }

namespace ThanuvaUi {

class CylinderWidget : public QWidget, public Ui::CylinderWidget
{
public:
    CylinderWidget(QWidget* parent)
        : QWidget{parent}
    {
        this->setupUi(this);
    }
};

class CylinderDialog : public GeometryDialog
{
    Q_OBJECT

public:
    CylinderDialog(QWidget* parent, Model::CylinderModel* cylinderModel);

private slots:
    void update();

private: // slots
    void initialize();

private:
    CylinderWidget* m_cylinderWidget{nullptr};
};

} // namespace ThanuvaUi

#endif // THANUVAUI_CYLINDERDIALOG_H
