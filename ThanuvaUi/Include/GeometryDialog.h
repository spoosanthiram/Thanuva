/*
 * ThanuvaUi: UI for Thanuva
 *
 * Copyright 2016, Saravanan Poosanthiram
 * All rights reserved.
 */

#ifndef THANUVAUI_GEOMETRYDIALOG_H
#define THANUVAUI_GEOMETRYDIALOG_H

#include "ui_GeometryDialog.h"

namespace Model { class ModelObject; }

namespace ThanuvaUi {

class GeometryDialog : public QDialog, private Ui::GeometryDialog
{
    Q_OBJECT

public:
    GeometryDialog(QWidget* parent, Model::ModelObject* modelObject);

    Model::ModelObject* modelObject() const { return m_modelObject; }
    QVBoxLayout* geometryPlaceHolderLayout() const { return m_geometryPlaceHolderLayout; }

    void setErrorText(const QString& text) { m_errorLabel->setText(text); }

private slots:
    bool updateModelName();
    void updateModelTransform();
    void done();

private: // slots
    void updateUiName();
    void updateUiTransform();

private:
    Model::ModelObject* m_modelObject;
};

} // namespace ThanuvaUi

#endif // THANUVAUI_GEOMETRYDIALOG_H
