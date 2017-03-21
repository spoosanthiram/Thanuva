/*
 * ThanuvaUi: UI for Thanuva
 *
 * Copyright 2016, Saravanan Poosanthiram
 * All rights reserved.
 */

#include "SphereDialog.h"

#include "SphereModel.h"

namespace ThanuvaUi {

SphereDialog::SphereDialog(QWidget* parent, Model::SphereModel* sphereModel)
    : GeometryDialog{parent, sphereModel}
    , m_sphereModel{sphereModel}
{
    m_sphereWidget = new SphereWidget{this};
    for (int i = 0; i < Model::SphereModel::kMaxSubdivisions; ++i)
        m_sphereWidget->subdivisionsComboBox->addItem(QString::number(i));

    this->geometryPlaceHolderLayout()->addWidget(m_sphereWidget);

    this->initCenter();
    this->initRadius();
    this->initSubdivisions();

    m_sphereModel->centerChanged.connect<SphereDialog, &SphereDialog::initCenter>(this);
    m_sphereModel->radiusChanged.connect<SphereDialog, &SphereDialog::initRadius>(this);
    m_sphereModel->subdivisionsChanged.connect<SphereDialog, &SphereDialog::initSubdivisions>(this);

    connect(m_sphereWidget->centerXLineEdit, &QLineEdit::editingFinished, this, &SphereDialog::updateCenter);
    connect(m_sphereWidget->centerYLineEdit, &QLineEdit::editingFinished, this, &SphereDialog::updateCenter);
    connect(m_sphereWidget->centerZLineEdit, &QLineEdit::editingFinished, this, &SphereDialog::updateCenter);

    connect(m_sphereWidget->radiusLineEdit, &QLineEdit::editingFinished, this, &SphereDialog::updateRadius);

    connect(m_sphereWidget->subdivisionsComboBox, SIGNAL(activated(int)), this, SLOT(updateSubdivisions()));
}

void SphereDialog::updateCenter()
{
    Core::Point3d point{};
    point.setX(m_sphereWidget->centerXLineEdit->text().toDouble());
    point.setY(m_sphereWidget->centerYLineEdit->text().toDouble());
    point.setZ(m_sphereWidget->centerZLineEdit->text().toDouble());
    m_sphereModel->setCenter(point);
}

void SphereDialog::updateRadius()
{
    m_sphereModel->setRadius(m_sphereWidget->radiusLineEdit->text().toDouble());
}

void SphereDialog::updateSubdivisions()
{
    m_sphereModel->setSubdivisions(m_sphereWidget->subdivisionsComboBox->currentIndex());
}

void SphereDialog::initCenter()
{
    auto& point = m_sphereModel->center();
    m_sphereWidget->centerXLineEdit->setText(QString::number(point.x()));
    m_sphereWidget->centerYLineEdit->setText(QString::number(point.y()));
    m_sphereWidget->centerZLineEdit->setText(QString::number(point.z()));
}

void SphereDialog::initRadius()
{
    m_sphereWidget->radiusLineEdit->setText(QString::number(m_sphereModel->radius()));
}

void SphereDialog::initSubdivisions()
{
    m_sphereWidget->subdivisionsComboBox->setCurrentIndex(m_sphereModel->subdivisions());
}

} // namespace ThanuvaUi
