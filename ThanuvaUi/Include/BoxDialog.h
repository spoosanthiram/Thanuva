/*
 * ThanuvaUi: UI for Thanuva
 *
 * Copyright 2016, Saravanan Poosanthiram
 * All rights reserved.
 */

#ifndef GLVIEWER_BOXDIALOG_H
#define GLVIEWER_BOXDIALOG_H

#include <memory>

#include "ui_BoxDialog.h"

namespace Model { class Box; }

namespace GlViewer {

class BoxDialog : public QDialog, private Ui::BoxDialog
{
    Q_OBJECT

public:
    BoxDialog(QWidget* parent, const std::shared_ptr<Model::Box>& box);

protected slots:
    void update();

protected: // slots
    void initialize();

private:
    std::shared_ptr<Model::Box> m_box;
};

} // namespace GlViewer

#endif // GLVIEWER_BOXDIALOG_H
