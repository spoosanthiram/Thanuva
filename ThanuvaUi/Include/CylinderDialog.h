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
    void updateEndpoint1();
    void updateRadius1();
    void updateEndpoint2();
    void updateRadius2();
    void updateNumFacets();

private: // slots
    void initEndpoint1();
    void initRadius1();
    void initEndpoint2();
    void initRadius2();
    void initNumFacets();

private:
    Model::CylinderModel* m_cylinderModel;
    CylinderWidget* m_cylinderWidget{nullptr};
};

} // namespace ThanuvaUi

#endif // THANUVAUI_CYLINDERDIALOG_H
