/*
 * ThanuvaUi: UI for Thanuva
 *
 * Copyright 2016, Saravanan Poosanthiram
 * All rights reserved.
 */

#ifndef ThanuvaUi_CylinderDialog_h
#define ThanuvaUi_CylinderDialog_h

#include "GeometryDialog.h"
#include "ui_CylinderDialog.h"

namespace Model { class CylinderModel; }

namespace ThanuvaUi {

class CylinderDialog : public GeometryDialog, private Ui::CylinderDialog
{
    Q_OBJECT

public:
    CylinderDialog(QWidget* parent, Model::CylinderModel* cylinderModel);

protected:
    QLabel* errorLabel() override { return m_errorLabel; }
    QLineEdit* nameLineEdit() override { return m_nameLineEdit; }
    QComboBox* csysComboBox() override { return m_csysComboBox; }
    QPushButton* doneButton() override { return m_doneButton; }

private slots:
    void updateModelEndpoint1();
    void updateModelRadius1();
    void updateModelEndpoint2();
    void updateModelRadius2();
    void updateModelNumFacets();

private: // slots
    void updateUiEndpoint1();
    void updateUiRadius1();
    void updateUiEndpoint2();
    void updateUiRadius2();
    void updateUiNumFacets();

private:
    Model::CylinderModel* m_cylinderModel;
};

} // namespace ThanuvaUi

#endif // ThanuvaUi_CylinderDialog_h
