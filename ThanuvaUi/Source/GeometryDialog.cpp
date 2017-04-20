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

namespace ThanuvaUi {

GeometryDialog::GeometryDialog(QWidget* parent, Model::GeometryModel* geometryModel)
    : QDialog{parent, Qt::WindowFlags() | Qt::WindowTitleHint | Qt::WindowSystemMenuHint}
    , m_geometryModel{geometryModel}
{
    CHECK(m_geometryModel);

    this->setupUi(this);
    this->setWindowOpacity(AppSettings().windowOpacity());

    this->updateUiName();
    this->updateUiTransform();

    m_geometryModel->nameChanged.connect<GeometryDialog, &GeometryDialog::updateUiName>(this);
    //m_geometryModel->transformChanged.connect<GeometryDialog, &GeometryDialog::updateUiTransform>(this);

    connect(m_nameLineEdit, &QLineEdit::editingFinished, this, &GeometryDialog::updateModelName);
    connect(m_translateXLineEdit, &QLineEdit::editingFinished, this, &GeometryDialog::updateModelTransform);
    connect(m_translateYLineEdit, &QLineEdit::editingFinished, this, &GeometryDialog::updateModelTransform);
    connect(m_translateZLineEdit, &QLineEdit::editingFinished, this, &GeometryDialog::updateModelTransform);

    connect(m_doneButton, &QPushButton::clicked, this, &GeometryDialog::done);
}

bool GeometryDialog::updateModelName()
{
    bool retval = true;
    try {
        m_geometryModel->setName(m_nameLineEdit->text().toStdString());
        this->setErrorText(QString{});
    }
    catch (const std::exception& e) {
        this->setErrorText(e.what());
        retval = false;
    }
    return retval;
}

void GeometryDialog::updateModelTransform()
{
    //Model::ModelObject::Transform xform;
    //xform.translateX = m_translateXLineEdit->text().toDouble();
    //xform.translateY = m_translateYLineEdit->text().toDouble();
    //xform.translateZ = m_translateZLineEdit->text().toDouble();

    //m_modelObject->setTransform(xform);
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
    m_nameLineEdit->setText(m_geometryModel->name().c_str());
}

void GeometryDialog::updateUiTransform()
{
    //auto xform = m_modelObject->transform();

    //m_translateXLineEdit->setText(QString::number(xform.translateX));
    //m_translateYLineEdit->setText(QString::number(xform.translateY));
    //m_translateZLineEdit->setText(QString::number(xform.translateZ));
}

} // namespace ThanuvaUi
