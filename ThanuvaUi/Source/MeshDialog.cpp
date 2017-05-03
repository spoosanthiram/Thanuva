/*
 * ThanuvaUi: UI for Thanuva
 *
 * Copyright 2016, Saravanan Poosanthiram
 * All rights reserved.
 */

#include "MeshDialog.h"

#include <QFileDialog>

#include "MeshModel.h"
#include "Scene.h"
#include "ThanuvaApp.h"

namespace ThanuvaUi {

MeshDialog::MeshDialog(QWidget* parent, Model::MeshModel* meshModel)
    : GeometryDialog{parent, meshModel}
    , m_meshModel{meshModel}
{
    this->setupUi(this);
    this->initialize(); // initialize GeometryDialog

    this->updateUiFilePath();

    m_meshModel->filePathChanged.connect<MeshDialog, &MeshDialog::updateUiFilePath>(this);

    connect(m_filePathLineEdit, &QLineEdit::textChanged, this, &MeshDialog::updateModelFilePath);
    connect(m_browseButton, &QPushButton::clicked, this, &MeshDialog::handleBrowse);
}

void MeshDialog::updateModelFilePath()
{
    m_meshModel->setFilePath(m_filePathLineEdit->text().toStdString());
}

void MeshDialog::handleBrowse()
{
    QString caption{"Import Mesh"};
    QString filePath = QFileDialog::getOpenFileName(
            this, caption, m_meshModel->scene()->thanuvaApp().recentDirPath().string().c_str(),
            "Mesh Files (*.nas *.off *.stl)");
    if (filePath.isEmpty())
        return;

    m_filePathLineEdit->setText(filePath);
}

void MeshDialog::updateUiFilePath()
{
    m_filePathLineEdit->setText(m_meshModel->filePath().string().c_str());
}

} // namespace ThanuvaUi
