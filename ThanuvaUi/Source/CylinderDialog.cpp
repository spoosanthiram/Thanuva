/*
 * ThanuvaUi: UI for Thanuva
 *
 * Copyright 2016, Saravanan Poosanthiram
 * All rights reserved.
 */

#include "CylinderDialog.h"

#include "CylinderModel.h"

namespace ThanuvaUi {

CylinderDialog::CylinderDialog(QWidget* parent, Model::CylinderModel* cylinderModel)
    : GeometryDialog{parent, cylinderModel}
    , m_cylinderModel{cylinderModel}
{
    this->setupUi(this);
    this->initialize(); // initialize GeometryDialog

    this->updateUiEndpoint1();
    this->updateUiRadius1();
    this->updateUiEndpoint2();
    this->updateUiRadius2();
    this->updateUiNumFacets();

    m_cylinderModel->endpoint1Changed.connect<CylinderDialog, &CylinderDialog::updateUiEndpoint1>(this);
    m_cylinderModel->radius1Changed.connect<CylinderDialog, &CylinderDialog::updateUiRadius1>(this);
    m_cylinderModel->endpoint2Changed.connect<CylinderDialog, &CylinderDialog::updateUiEndpoint2>(this);
    m_cylinderModel->radius2Changed.connect<CylinderDialog, &CylinderDialog::updateUiRadius2>(this);
    m_cylinderModel->numFacetsChanged.connect<CylinderDialog, &CylinderDialog::updateUiNumFacets>(this);

    connect(m_endpoint1XLineEdit, &QLineEdit::editingFinished, this, &CylinderDialog::updateModelEndpoint1);
    connect(m_endpoint1YLineEdit, &QLineEdit::editingFinished, this, &CylinderDialog::updateModelEndpoint1);
    connect(m_endpoint1ZLineEdit, &QLineEdit::editingFinished, this, &CylinderDialog::updateModelEndpoint1);

    connect(m_radius1LineEdit, &QLineEdit::editingFinished, this, &CylinderDialog::updateModelRadius1);

    connect(m_endpoint2XLineEdit, &QLineEdit::editingFinished, this, &CylinderDialog::updateModelEndpoint2);
    connect(m_endpoint2YLineEdit, &QLineEdit::editingFinished, this, &CylinderDialog::updateModelEndpoint2);
    connect(m_endpoint2ZLineEdit, &QLineEdit::editingFinished, this, &CylinderDialog::updateModelEndpoint2);

    connect(m_radius2LineEdit, &QLineEdit::editingFinished, this, &CylinderDialog::updateModelRadius2);

    connect(m_numFacetsLineEdit, &QLineEdit::editingFinished, this, &CylinderDialog::updateModelNumFacets);
}

void CylinderDialog::updateModelEndpoint1()
{
    Core::Point3d point{};
    point.setX(m_endpoint1XLineEdit->text().toDouble());
    point.setY(m_endpoint1YLineEdit->text().toDouble());
    point.setZ(m_endpoint1ZLineEdit->text().toDouble());
    m_cylinderModel->setEndpoint1(point);
}

void CylinderDialog::updateModelRadius1()
{
    m_cylinderModel->setRadius1(m_radius1LineEdit->text().toDouble());
}

void CylinderDialog::updateModelEndpoint2()
{
    Core::Point3d point{};
    point.setX(m_endpoint2XLineEdit->text().toDouble());
    point.setY(m_endpoint2YLineEdit->text().toDouble());
    point.setZ(m_endpoint2ZLineEdit->text().toDouble());
    m_cylinderModel->setEndpoint2(point);
}

void CylinderDialog::updateModelRadius2()
{
    m_cylinderModel->setRadius2(m_radius2LineEdit->text().toDouble());
}

void CylinderDialog::updateModelNumFacets()
{
    m_cylinderModel->setNumFacets(m_numFacetsLineEdit->text().toDouble());
}

void CylinderDialog::updateUiEndpoint1()
{
    auto& point = m_cylinderModel->endpoint1();
    m_endpoint1XLineEdit->setText(QString::number(point.x()));
    m_endpoint1YLineEdit->setText(QString::number(point.y()));
    m_endpoint1ZLineEdit->setText(QString::number(point.z()));
}

void CylinderDialog::updateUiRadius1()
{
    m_radius1LineEdit->setText(QString::number(m_cylinderModel->radius1()));
}

void CylinderDialog::updateUiEndpoint2()
{
    auto& point = m_cylinderModel->endpoint2();
    m_endpoint2XLineEdit->setText(QString::number(point.x()));
    m_endpoint2YLineEdit->setText(QString::number(point.y()));
    m_endpoint2ZLineEdit->setText(QString::number(point.z()));
}

void CylinderDialog::updateUiRadius2()
{
    m_radius2LineEdit->setText(QString::number(m_cylinderModel->radius2()));
}

void CylinderDialog::updateUiNumFacets()
{
    m_numFacetsLineEdit->setText(QString::number(m_cylinderModel->numFacets()));
}

} // namespace ThanuvaUi
