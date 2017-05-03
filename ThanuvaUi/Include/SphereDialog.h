/*
 * ThanuvaUi: UI for Thanuva
 *
 * Copyright 2016, Saravanan Poosanthiram
 * All rights reserved.
 */

#ifndef ThanuvaUi_SphereDialog_h
#define ThanuvaUi_SphereDialog_h

#include "GeometryDialog.h"
#include "ui_SphereDialog.h"

namespace Model { class SphereModel; }

namespace ThanuvaUi {

class SphereDialog : public GeometryDialog, private Ui::SphereDialog
{
    Q_OBJECT

public:
    SphereDialog(QWidget* parent, Model::SphereModel* sphereModel);

protected:
    QLabel* errorLabel() override { return m_errorLabel; }
    QLineEdit* nameLineEdit() override { return m_nameLineEdit; }
    QComboBox* csysComboBox() override { return m_csysComboBox; }
    QPushButton* doneButton() override { return m_doneButton; }

private slots:
    void updateModelCenter();
    void updateModelRadius();
    void updateModelSubdivisions();

private: // slots
    void updateUiCenter();
    void updateUiRadius();
    void updateUiSubdivisions();

private:
    Model::SphereModel* m_sphereModel;
};

} // namespace ThanuvaUi

#endif // ThanuvaUi_SphereDialog_h
