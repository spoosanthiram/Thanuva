/*
 * ThanuvaUi: UI for Thanuva
 *
 * Copyright 2016, Saravanan Poosanthiram
 * All rights reserved.
 */

#ifndef ThanuvaUi_BoxDialog_h
#define ThanuvaUi_BoxDialog_h

#include "GeometryDialog.h"
#include "ui_BoxDialog.h"

namespace Model { class BoxModel; }

namespace ThanuvaUi {

class BoxDialog : public GeometryDialog, private Ui::BoxDialog
{
    Q_OBJECT

public:
    BoxDialog(QWidget* parent, Model::BoxModel* boxModel);

protected:
    QLabel* errorLabel() override { return m_errorLabel; }
    QLineEdit* nameLineEdit() override { return m_nameLineEdit; }
    QComboBox* csysComboBox() override { return m_csysComboBox; }
    QPushButton* doneButton() override { return m_doneButton; }

private slots:
    void updateModelLimiter();

private: // slots
    void updateUiLimiter();

private:
    Model::BoxModel* m_boxModel;
};

} // namespace ThanuvaUi

#endif // ThanuvaUi_BoxDialog_h
