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
    this->setupUi(this);
    this->initialize(); // initialize GeometryDialog

    for (int i = 0; i < Model::SphereModel::kMaxSubdivisions; ++i)
        m_subdivisionsComboBox->addItem(QString::number(i));

    this->updateUiCenter();
    this->updateUiRadius();
    this->updateUiSubdivisions();

    m_sphereModel->centerChanged.connect<SphereDialog, &SphereDialog::updateUiCenter>(this);
    m_sphereModel->radiusChanged.connect<SphereDialog, &SphereDialog::updateUiRadius>(this);
    m_sphereModel->subdivisionsChanged.connect<SphereDialog, &SphereDialog::updateUiSubdivisions>(this);

    connect(m_centerXLineEdit, &QLineEdit::editingFinished, this, &SphereDialog::updateModelCenter);
    connect(m_centerYLineEdit, &QLineEdit::editingFinished, this, &SphereDialog::updateModelCenter);
    connect(m_centerZLineEdit, &QLineEdit::editingFinished, this, &SphereDialog::updateModelCenter);

    connect(m_radiusLineEdit, &QLineEdit::editingFinished, this, &SphereDialog::updateModelRadius);

    connect(m_subdivisionsComboBox, SIGNAL(activated(int)), this, SLOT(updateModelSubdivisions()));
}

void SphereDialog::updateModelCenter()
{
    Core::Point3d point{};
    point.setX(m_centerXLineEdit->text().toDouble());
    point.setY(m_centerYLineEdit->text().toDouble());
    point.setZ(m_centerZLineEdit->text().toDouble());
    m_sphereModel->setCenter(point);
}

void SphereDialog::updateModelRadius()
{
    m_sphereModel->setRadius(m_radiusLineEdit->text().toDouble());
}

void SphereDialog::updateModelSubdivisions()
{
    m_sphereModel->setSubdivisions(m_subdivisionsComboBox->currentIndex());
}

void SphereDialog::updateUiCenter()
{
    auto& point = m_sphereModel->center();
    m_centerXLineEdit->setText(QString::number(point.x()));
    m_centerYLineEdit->setText(QString::number(point.y()));
    m_centerZLineEdit->setText(QString::number(point.z()));
}

void SphereDialog::updateUiRadius()
{
    m_radiusLineEdit->setText(QString::number(m_sphereModel->radius()));
}

void SphereDialog::updateUiSubdivisions()
{
    m_subdivisionsComboBox->setCurrentIndex(m_sphereModel->subdivisions());
}

} // namespace ThanuvaUi
