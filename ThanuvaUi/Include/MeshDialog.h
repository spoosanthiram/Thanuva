/*
 * ThanuvaUi: UI for Thanuva
 *
 * Copyright 2016, Saravanan Poosanthiram
 * All rights reserved.
 */

#ifndef ThanuvaUi_MeshDialog_h
#define ThanuvaUi_MeshDialog_h

#include "GeometryDialog.h"
#include "ui_MeshDialog.h"

namespace Model { class MeshModel; }

namespace ThanuvaUi {

class MeshDialog : public GeometryDialog, private Ui::MeshDialog
{
    Q_OBJECT

public:
    MeshDialog(QWidget* parent, Model::MeshModel* meshModel);

protected:
    QLabel* errorLabel() override { return m_errorLabel; }
    QLineEdit* nameLineEdit() override { return m_nameLineEdit; }
    QComboBox* csysComboBox() override { return m_csysComboBox; }
    QPushButton* doneButton() override { return m_doneButton; }

private slots:
    void updateModelFilePath();
    void handleBrowse();

private: // slots
    void updateUiFilePath();

private:
    Model::MeshModel* m_meshModel;
};

} // namespace ThanuvaUi

#endif // ThanuvaUi_MeshDialog_h
