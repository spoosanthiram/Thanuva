/*
 * ThanuvaUi: UI for Thanuva
 *
 * Copyright 2016, Saravanan Poosanthiram
 * All rights reserved.
 */

#ifndef ThanuvaUi_GeometryDialog_h
#define ThanuvaUi_GeometryDialog_h

#include <QComboBox>
#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>

namespace Model { class GeometryModel; }

namespace ThanuvaUi {

class GeometryDialog : public QDialog
{
    Q_OBJECT

public:
    GeometryDialog(QWidget* parent, Model::GeometryModel* geometryModel);

    Model::GeometryModel* geometryModel() const { return m_geometryModel; }

    void setErrorText(const QString& text) { this->errorLabel()->setText(text); }

protected:
    void initialize();

    virtual QLabel* errorLabel() = 0;
    virtual QLineEdit* nameLineEdit() = 0;
    virtual QComboBox* csysComboBox() = 0;
    virtual QPushButton* doneButton() = 0;

private slots:
    bool updateModelName();
    void updateModelCoordnateSystem(int index);
    void done();

private: // slots
    void updateUiName();
    void updateUiCoordnateSystemList();
    void updateUiCoordnateSystem();

private:
    Model::GeometryModel* m_geometryModel;
};

} // namespace ThanuvaUi

#endif // ThanuvaUi_GeometryDialog_h
