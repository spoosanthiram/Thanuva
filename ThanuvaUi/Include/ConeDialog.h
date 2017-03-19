/*
 * ThanuvaUi: UI for Thanuva
 *
 * Copyright 2016, Saravanan Poosanthiram
 * All rights reserved.
 */

#ifndef THANUVAUI_CONEDIALOG_H
#define THANUVAUI_CONEDIALOG_H

#include "GeometryDialog.h"
#include "ui_ConeWidget.h"

namespace Model { class ConeModel; }

namespace ThanuvaUi {

class ConeWidget : public QWidget, public Ui::ConeWidget
{
public:
    ConeWidget(QWidget* parent)
        : QWidget{parent}
    {
        this->setupUi(this);
    }
};

class ConeDialog : public GeometryDialog
{
    Q_OBJECT

public:
    ConeDialog(QWidget* parent, Model::ConeModel* coneModel);

private slots:
    void updateApex();
    void updateCenter();
    void updateRadius();
    void updateNumFacets();

private: // slots
    void initApex();
    void initCenter();
    void initRadius();
    void initNumFacets();

private:
    Model::ConeModel* m_coneModel;
    ConeWidget* m_coneWidget{nullptr};
};

} // namespace ThanuvaUi

#endif // THANUVAUI_CONEDIALOG_H
