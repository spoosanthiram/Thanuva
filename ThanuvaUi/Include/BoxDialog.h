/*
 * ThanuvaUi: UI for Thanuva
 *
 * Copyright 2016, Saravanan Poosanthiram
 * All rights reserved.
 */

#ifndef THANUVAUI_BOXDIALOG_H
#define THANUVAUI_BOXDIALOG_H

#include "GeometryDialog.h"
#include "ui_BoxWidget.h"

namespace Model { class BoxModel; }

namespace ThanuvaUi {

class BoxWidget : public QWidget, public Ui::BoxWidget
{
public:
    BoxWidget(QWidget* parent)
        : QWidget{parent}
    {
        this->setupUi(this);
    }
};

class BoxDialog : public GeometryDialog
{
    Q_OBJECT

public:
    BoxDialog(QWidget* parent, Model::BoxModel* boxModel);

private slots:
    void update();

private: // slots
    void initialize();

private:
    Model::BoxModel* m_boxModel;
    BoxWidget* m_boxWidget{nullptr};
};

} // namespace ThanuvaUi

#endif // THANUVAUI_BOXDIALOG_H
