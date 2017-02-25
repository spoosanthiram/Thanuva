/*
 * ThanuvaUi: UI for Thanuva
 *
 * Copyright 2016, Saravanan Poosanthiram
 * All rights reserved.
 */

#include "CylinderDialog.h"

#include <QMessageBox>

#include "CylinderModel.h"

namespace ThanuvaUi {

CylinderDialog::CylinderDialog(QWidget* parent, Model::CylinderModel* cylinderModel)
    : GeometryDialog{parent, cylinderModel}
    , m_cylinderModel{cylinderModel}
{
    m_cylinderWidget = new CylinderWidget{this};
    this->geometryPlaceHolderLayout()->addWidget(m_cylinderWidget);

    this->initEndpoint1();
    this->initRadius1();
    this->initEndpoint2();
    this->initRadius2();
    this->initNumFacets();

    m_cylinderModel->endpoint1Changed.connect<CylinderDialog, &CylinderDialog::initEndpoint1>(this);
    m_cylinderModel->radius1Changed.connect<CylinderDialog, &CylinderDialog::initRadius1>(this);
    m_cylinderModel->endpoint2Changed.connect<CylinderDialog, &CylinderDialog::initEndpoint2>(this);
    m_cylinderModel->radius2Changed.connect<CylinderDialog, &CylinderDialog::initRadius2>(this);
    m_cylinderModel->numFacetsChanged.connect<CylinderDialog, &CylinderDialog::initNumFacets>(this);

    connect(m_cylinderWidget->endpoint1XLineEdit, &QLineEdit::editingFinished, this, &CylinderDialog::updateEndpoint1);
    connect(m_cylinderWidget->endpoint1YLineEdit, &QLineEdit::editingFinished, this, &CylinderDialog::updateEndpoint1);
    connect(m_cylinderWidget->endpoint1ZLineEdit, &QLineEdit::editingFinished, this, &CylinderDialog::updateEndpoint1);

    connect(m_cylinderWidget->radius1LineEdit, &QLineEdit::editingFinished, this, &CylinderDialog::updateRadius1);

    connect(m_cylinderWidget->endpoint2XLineEdit, &QLineEdit::editingFinished, this, &CylinderDialog::updateEndpoint2);
    connect(m_cylinderWidget->endpoint2YLineEdit, &QLineEdit::editingFinished, this, &CylinderDialog::updateEndpoint2);
    connect(m_cylinderWidget->endpoint2ZLineEdit, &QLineEdit::editingFinished, this, &CylinderDialog::updateEndpoint2);

    connect(m_cylinderWidget->radius2LineEdit, &QLineEdit::editingFinished, this, &CylinderDialog::updateRadius2);

    connect(m_cylinderWidget->numFacetsLineEdit, &QLineEdit::editingFinished, this, &CylinderDialog::updateNumFacets);
}

void CylinderDialog::updateEndpoint1()
{
    Core::Point3d point{};
    point.setX(m_cylinderWidget->endpoint1XLineEdit->text().toDouble());
    point.setY(m_cylinderWidget->endpoint1YLineEdit->text().toDouble());
    point.setZ(m_cylinderWidget->endpoint1ZLineEdit->text().toDouble());
    m_cylinderModel->setEndpoint1(point);
}

void CylinderDialog::updateRadius1()
{
    m_cylinderModel->setRadius1(m_cylinderWidget->radius1LineEdit->text().toDouble());
}

void CylinderDialog::updateEndpoint2()
{
    Core::Point3d point{};
    point.setX(m_cylinderWidget->endpoint2XLineEdit->text().toDouble());
    point.setY(m_cylinderWidget->endpoint2YLineEdit->text().toDouble());
    point.setZ(m_cylinderWidget->endpoint2ZLineEdit->text().toDouble());
    m_cylinderModel->setEndpoint2(point);
}

void CylinderDialog::updateRadius2()
{
    m_cylinderModel->setRadius2(m_cylinderWidget->radius2LineEdit->text().toDouble());
}

void CylinderDialog::updateNumFacets()
{
    m_cylinderModel->setNumFacets(m_cylinderWidget->numFacetsLineEdit->text().toDouble());
}

void CylinderDialog::initEndpoint1()
{
    auto& point = m_cylinderModel->endpoint1();
    m_cylinderWidget->endpoint1XLineEdit->setText(QString::number(point.x()));
    m_cylinderWidget->endpoint1YLineEdit->setText(QString::number(point.y()));
    m_cylinderWidget->endpoint1ZLineEdit->setText(QString::number(point.z()));
}

void CylinderDialog::initRadius1()
{
    m_cylinderWidget->radius1LineEdit->setText(QString::number(m_cylinderModel->radius1()));
}

void CylinderDialog::initEndpoint2()
{
    auto& point = m_cylinderModel->endpoint2();
    m_cylinderWidget->endpoint2XLineEdit->setText(QString::number(point.x()));
    m_cylinderWidget->endpoint2YLineEdit->setText(QString::number(point.y()));
    m_cylinderWidget->endpoint2ZLineEdit->setText(QString::number(point.z()));
}

void CylinderDialog::initRadius2()
{
    m_cylinderWidget->radius2LineEdit->setText(QString::number(m_cylinderModel->radius2()));
}

void CylinderDialog::initNumFacets()
{
    m_cylinderWidget->numFacetsLineEdit->setText(QString::number(m_cylinderModel->numFacets()));
}

} // namespace ThanuvaUi
