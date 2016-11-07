/*
 * ThanuvaUi: UI for Thanuva
 *
 * Copyright 2016, Saravanan Poosanthiram
 * All rights reserved.
 */

#ifndef THANUVAUI_BOXDIALOG_H
#define THANUVAUI_BOXDIALOG_H

#include <memory>

#include "ui_BoxDialog.h"

namespace Model { class BoxModel; }

namespace ThanuvaUi {

class BoxDialog : public QDialog, private Ui::BoxDialog
{
    Q_OBJECT

public:
    BoxDialog(QWidget* parent, Model::BoxModel* boxModel);

protected slots:
    void update();

protected: // slots
    void initialize();

private:
    Model::BoxModel* m_boxModel;
};

} // namespace ThanuvaUi

#endif // THANUVAUI_BOXDIALOG_H
