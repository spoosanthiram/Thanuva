/*
 * ThanuvaUi: UI for Thanuva
 *
 * Copyright 2016, Saravanan Poosanthiram
 * All rights reserved.
 */

#include "ConeDialog.h"

#include "ConeModel.h"

namespace ThanuvaUi {

ConeDialog::ConeDialog(QWidget* parent, Model::ConeModel* coneModel)
    : GeometryDialog{parent, coneModel}
    , m_coneModel{coneModel}
{
    this->setupUi(this);
    this->initialize(); // initialize GeometryDialog

    this->updateUiApex();
    this->updateUiCenter();
    this->updateUiRadius();
    this->updateUiNumFacets();

    m_coneModel->apexChanged.connect<ConeDialog, &ConeDialog::updateUiApex>(this);
    m_coneModel->centerChanged.connect<ConeDialog, &ConeDialog::updateUiCenter>(this);
    m_coneModel->radiusChanged.connect<ConeDialog, &ConeDialog::updateUiRadius>(this);
    m_coneModel->numFacetsChanged.connect<ConeDialog, &ConeDialog::updateUiNumFacets>(this);

    connect(m_apexXLineEdit, &QLineEdit::editingFinished, this, &ConeDialog::updateModelApex);
    connect(m_apexYLineEdit, &QLineEdit::editingFinished, this, &ConeDialog::updateModelApex);
    connect(m_apexZLineEdit, &QLineEdit::editingFinished, this, &ConeDialog::updateModelApex);

    connect(m_centerXLineEdit, &QLineEdit::editingFinished, this, &ConeDialog::updateModelCenter);
    connect(m_centerYLineEdit, &QLineEdit::editingFinished, this, &ConeDialog::updateModelCenter);
    connect(m_centerZLineEdit, &QLineEdit::editingFinished, this, &ConeDialog::updateModelCenter);

    connect(m_radiusLineEdit, &QLineEdit::editingFinished, this, &ConeDialog::updateModelRadius);

    connect(m_numFacetsLineEdit, &QLineEdit::editingFinished, this, &ConeDialog::updateModelNumFacets);
}

void ConeDialog::updateModelApex()
{
    Core::Point3d point{};
    point.setX(m_apexXLineEdit->text().toDouble());
    point.setY(m_apexYLineEdit->text().toDouble());
    point.setZ(m_apexZLineEdit->text().toDouble());
    m_coneModel->setApex(point);
}

void ConeDialog::updateModelCenter()
{
    Core::Point3d point{};
    point.setX(m_centerXLineEdit->text().toDouble());
    point.setY(m_centerYLineEdit->text().toDouble());
    point.setZ(m_centerZLineEdit->text().toDouble());
    m_coneModel->setCenter(point);
}

void ConeDialog::updateModelRadius()
{
    m_coneModel->setRadius(m_radiusLineEdit->text().toDouble());
}

void ConeDialog::updateModelNumFacets()
{
    m_coneModel->setNumFacets(m_numFacetsLineEdit->text().toDouble());
}

void ConeDialog::updateUiApex()
{
    auto& point = m_coneModel->apex();
    m_apexXLineEdit->setText(QString::number(point.x()));
    m_apexYLineEdit->setText(QString::number(point.y()));
    m_apexZLineEdit->setText(QString::number(point.z()));
}

void ConeDialog::updateUiCenter()
{
    auto& point = m_coneModel->center();
    m_centerXLineEdit->setText(QString::number(point.x()));
    m_centerYLineEdit->setText(QString::number(point.y()));
    m_centerZLineEdit->setText(QString::number(point.z()));
}

void ConeDialog::updateUiRadius()
{
    m_radiusLineEdit->setText(QString::number(m_coneModel->radius()));
}

void ConeDialog::updateUiNumFacets()
{
    m_numFacetsLineEdit->setText(QString::number(m_coneModel->numFacets()));
}

} // namespace ThanuvaUi
