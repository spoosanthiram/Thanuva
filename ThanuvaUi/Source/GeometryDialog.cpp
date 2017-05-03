/*
 * ThanuvaUi: UI for Thanuva
 *
 * Copyright 2016, Saravanan Poosanthiram
 * All rights reserved.
 */

#include "GeometryDialog.h"

#include <glog/logging.h>

#include "AppSettings.h"
#include "GeometryModel.h"
#include "Scene.h"

namespace ThanuvaUi {

GeometryDialog::GeometryDialog(QWidget* parent, Model::GeometryModel* geometryModel)
    : QDialog{parent, Qt::WindowFlags() | Qt::WindowTitleHint | Qt::WindowSystemMenuHint}
    , m_geometryModel{geometryModel}
{
    CHECK(m_geometryModel);

    this->setWindowOpacity(AppSettings().windowOpacity());
}

void GeometryDialog::initialize()
{
    this->updateUiName();
    this->updateUiCoordnateSystemList();
    this->updateUiCoordnateSystem();

    m_geometryModel->nameChanged.connect<GeometryDialog, &GeometryDialog::updateUiName>(this);
    m_geometryModel->coordinateSystemModelChanged.connect<GeometryDialog, &GeometryDialog::updateUiCoordnateSystem>(this);

    connect(this->nameLineEdit(), &QLineEdit::editingFinished, this, &GeometryDialog::updateModelName);
    connect(this->csysComboBox(), SIGNAL(activated(int)), this, SLOT(updateModelCoordnateSystem(int)));

    connect(this->doneButton(), &QPushButton::clicked, this, &GeometryDialog::done);
}

bool GeometryDialog::updateModelName()
{
    bool retval = true;
    try {
        m_geometryModel->setName(this->nameLineEdit()->text().toStdString());
        this->setErrorText(QString{});
    }
    catch (const std::exception& e) {
        this->setErrorText(e.what());
        retval = false;
    }
    return retval;
}

void GeometryDialog::updateModelCoordnateSystem(int index)
{
    if (index == -1)
        return;

    auto& csysList = m_geometryModel->scene()->coordinateSystemModelList();
    if (index < csysList.size())
        m_geometryModel->setCoordinateSystemModel(csysList[index].get());
}

void GeometryDialog::done()
{
    // make sure name is not the unspecified one!
    if (!this->updateModelName())
        return;

    this->accept();
}

void GeometryDialog::updateUiName()
{
    this->nameLineEdit()->setText(m_geometryModel->name().c_str());
}

void GeometryDialog::updateUiCoordnateSystemList()
{
    QComboBox* csysComboBox = this->csysComboBox();
    csysComboBox->clear();
    for (auto& csys : m_geometryModel->scene()->coordinateSystemModelList())
        csysComboBox->addItem(csys->name().c_str());
}

void GeometryDialog::updateUiCoordnateSystem()
{
    auto index = m_geometryModel->scene()->csysModelIndex(m_geometryModel->coordinateSystemModel());
    if (index != -1)
        this->csysComboBox()->setCurrentIndex(index);
}

} // namespace ThanuvaUi
