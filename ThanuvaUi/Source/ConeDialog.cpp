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
    m_coneWidget = new ConeWidget{this};
    this->geometryPlaceHolderLayout()->addWidget(m_coneWidget);

    this->initApex();
    this->initCenter();
    this->initRadius();
    this->initNumFacets();

    m_coneModel->apexChanged.connect<ConeDialog, &ConeDialog::initApex>(this);
    m_coneModel->centerChanged.connect<ConeDialog, &ConeDialog::initCenter>(this);
    m_coneModel->radiusChanged.connect<ConeDialog, &ConeDialog::initRadius>(this);
    m_coneModel->numFacetsChanged.connect<ConeDialog, &ConeDialog::initNumFacets>(this);

    connect(m_coneWidget->apexXLineEdit, &QLineEdit::editingFinished, this, &ConeDialog::updateApex);
    connect(m_coneWidget->apexYLineEdit, &QLineEdit::editingFinished, this, &ConeDialog::updateApex);
    connect(m_coneWidget->apexZLineEdit, &QLineEdit::editingFinished, this, &ConeDialog::updateApex);

    connect(m_coneWidget->centerXLineEdit, &QLineEdit::editingFinished, this, &ConeDialog::updateCenter);
    connect(m_coneWidget->centerYLineEdit, &QLineEdit::editingFinished, this, &ConeDialog::updateCenter);
    connect(m_coneWidget->centerZLineEdit, &QLineEdit::editingFinished, this, &ConeDialog::updateCenter);

    connect(m_coneWidget->radiusLineEdit, &QLineEdit::editingFinished, this, &ConeDialog::updateRadius);

    connect(m_coneWidget->numFacetsLineEdit, &QLineEdit::editingFinished, this, &ConeDialog::updateNumFacets);
}

void ConeDialog::updateApex()
{
    Core::Point3d point{};
    point.setX(m_coneWidget->apexXLineEdit->text().toDouble());
    point.setY(m_coneWidget->apexYLineEdit->text().toDouble());
    point.setZ(m_coneWidget->apexZLineEdit->text().toDouble());
    m_coneModel->setApex(point);
}

void ConeDialog::updateCenter()
{
    Core::Point3d point{};
    point.setX(m_coneWidget->centerXLineEdit->text().toDouble());
    point.setY(m_coneWidget->centerYLineEdit->text().toDouble());
    point.setZ(m_coneWidget->centerZLineEdit->text().toDouble());
    m_coneModel->setCenter(point);
}

void ConeDialog::updateRadius()
{
    m_coneModel->setRadius(m_coneWidget->radiusLineEdit->text().toDouble());
}

void ConeDialog::updateNumFacets()
{
    m_coneModel->setNumFacets(m_coneWidget->numFacetsLineEdit->text().toDouble());
}

void ConeDialog::initApex()
{
    auto& point = m_coneModel->apex();
    m_coneWidget->apexXLineEdit->setText(QString::number(point.x()));
    m_coneWidget->apexYLineEdit->setText(QString::number(point.y()));
    m_coneWidget->apexZLineEdit->setText(QString::number(point.z()));
}

void ConeDialog::initCenter()
{
    auto& point = m_coneModel->center();
    m_coneWidget->centerXLineEdit->setText(QString::number(point.x()));
    m_coneWidget->centerYLineEdit->setText(QString::number(point.y()));
    m_coneWidget->centerZLineEdit->setText(QString::number(point.z()));
}

void ConeDialog::initRadius()
{
    m_coneWidget->radiusLineEdit->setText(QString::number(m_coneModel->radius()));
}

void ConeDialog::initNumFacets()
{
    m_coneWidget->numFacetsLineEdit->setText(QString::number(m_coneModel->numFacets()));
}

} // namespace ThanuvaUi
