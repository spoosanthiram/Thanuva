/*
 * ThanuvaUi: UI for Thanuva
 *
 * Copyright 2016, Saravanan Poosanthiram
 * All rights reserved.
 */

#ifndef THANUVAUI_SPHEREDIALOG_H
#define THANUVAUI_SPHEREDIALOG_H

#include "GeometryDialog.h"
#include "ui_SphereWidget.h"

namespace Model { class SphereModel; }

namespace ThanuvaUi {

class SphereWidget : public QWidget, public Ui::SphereWidget
{
public:
    SphereWidget(QWidget* parent)
        : QWidget{parent}
    {
        this->setupUi(this);
    }
};

class SphereDialog : public GeometryDialog
{
    Q_OBJECT

public:
    SphereDialog(QWidget* parent, Model::SphereModel* sphereModel);

private slots:
    void updateCenter();
    void updateRadius();
    void updateSubdivisions();

private: // slots
    void initCenter();
    void initRadius();
    void initSubdivisions();

private:
    Model::SphereModel* m_sphereModel;
    SphereWidget* m_sphereWidget{nullptr};
};

} // namespace ThanuvaUi

#endif // THANUVAUI_SPHEREDIALOG_H
