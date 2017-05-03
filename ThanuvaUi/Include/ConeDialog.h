/*
 * ThanuvaUi: UI for Thanuva
 *
 * Copyright 2016, Saravanan Poosanthiram
 * All rights reserved.
 */

#ifndef ThanuvaUi_ConeDialog_h
#define ThanuvaUi_ConeDialog_h

#include "GeometryDialog.h"
#include "ui_ConeDialog.h"

namespace Model { class ConeModel; }

namespace ThanuvaUi {

class ConeDialog : public GeometryDialog, private Ui::ConeDialog
{
    Q_OBJECT

public:
    ConeDialog(QWidget* parent, Model::ConeModel* coneModel);


protected:
    QLabel* errorLabel() override { return m_errorLabel; }
    QLineEdit* nameLineEdit() override { return m_nameLineEdit; }
    QComboBox* csysComboBox() override { return m_csysComboBox; }
    QPushButton* doneButton() override { return m_doneButton; }

private slots:
    void updateModelApex();
    void updateModelCenter();
    void updateModelRadius();
    void updateModelNumFacets();

private: // slots
    void updateUiApex();
    void updateUiCenter();
    void updateUiRadius();
    void updateUiNumFacets();

private:
    Model::ConeModel* m_coneModel;
};

} // namespace ThanuvaUi

#endif // ThanuvaUi_ConeDialog_h
