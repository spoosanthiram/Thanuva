/*
 * ThanuvaUi: UI for Thanuva
 *
 * Copyright 2016, Saravanan Poosanthiram
 * All rights reserved.
 */

#include "GeometryDialog.h"

#include <glog/logging.h>

#include "AppSettings.h"
#include "ModelObject.h"

namespace ThanuvaUi {

GeometryDialog::GeometryDialog(QWidget* parent, Model::ModelObject* modelObject)
    : QDialog{parent, Qt::WindowFlags() | Qt::WindowTitleHint | Qt::WindowSystemMenuHint}
    , m_modelObject{modelObject}
{
    CHECK(modelObject);

    this->setupUi(this);
    this->setWindowOpacity(AppSettings().windowOpacity());

    this->initialize();

    m_modelObject->nameChanged.connect<GeometryDialog, &GeometryDialog::initialize>(this);

    connect(m_nameLineEdit, &QLineEdit::editingFinished, this, &GeometryDialog::update);
    connect(m_doneButton, &QPushButton::clicked, this, &GeometryDialog::done);
}

bool GeometryDialog::update()
{
    bool retval = true;
    try {
        m_modelObject->setName(m_nameLineEdit->text().toStdString());
        this->setErrorText(QString{});
    }
    catch (const std::exception& e) {
        this->setErrorText(e.what());
        retval = false;
    }
    return retval;
}

void GeometryDialog::done()
{
    // make sure name is not the unspecified one!
    if (!this->update())
        return;

    this->accept();
}

void GeometryDialog::initialize()
{
    m_nameLineEdit->setText(m_modelObject->name().c_str());
}

} // namespace ThanuvaUi
