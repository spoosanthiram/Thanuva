/**
* Author: Saravanan Poosanthiram
* $LastChangedBy: ps $
* $LastChangedDate: 2015-03-14 12:57:44 -0400 (Sat, 14 Mar 2015) $
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
