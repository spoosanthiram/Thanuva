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
{
    m_cylinderWidget = new CylinderWidget{this};
    this->geometryPlaceHolderLayout()->addWidget(m_cylinderWidget);

    this->initialize();
}

void CylinderDialog::update()
{
    double endpoint1X = m_cylinderWidget->endpoint1XLineEdit->text().toDouble();

    try {
        //dynamic_cast<Model::CylinderModel*>(this->modelObject())->set(endpoint1X);
        this->setErrorText(QString{});
    }
    catch (const std::exception& e) {
        this->setErrorText(e.what());
    }
}

void CylinderDialog::initialize()
{
    Model::CylinderModel* cylinderModel = dynamic_cast<Model::CylinderModel*>(this->modelObject());
}

} // namespace ThanuvaUi
