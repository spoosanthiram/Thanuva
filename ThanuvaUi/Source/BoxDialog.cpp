/*
 * ThanuvaUi: UI for Thanuva
 *
 * Copyright 2016, Saravanan Poosanthiram
 * All rights reserved.
 */

#include "BoxDialog.h"

#include <QMessageBox>

#include "BoxModel.h"

namespace ThanuvaUi {

BoxDialog::BoxDialog(QWidget* parent, Model::BoxModel* boxModel)
    : GeometryDialog{parent, boxModel}
    , m_boxModel{boxModel}
{
    m_boxWidget = new BoxWidget{this};
    this->geometryPlaceHolderLayout()->addWidget(m_boxWidget);

    this->initialize();

    m_boxModel->limiterChanged.connect<BoxDialog, &BoxDialog::initialize>(this);

    connect(m_boxWidget->xLowLineEdit, &QLineEdit::editingFinished, this, &BoxDialog::update);
    connect(m_boxWidget->xHighLineEdit, &QLineEdit::editingFinished, this, &BoxDialog::update);
    connect(m_boxWidget->yLowLineEdit, &QLineEdit::editingFinished, this, &BoxDialog::update);
    connect(m_boxWidget->yHighLineEdit, &QLineEdit::editingFinished, this, &BoxDialog::update);
    connect(m_boxWidget->zLowLineEdit, &QLineEdit::editingFinished, this, &BoxDialog::update);
    connect(m_boxWidget->zHighLineEdit, &QLineEdit::editingFinished, this, &BoxDialog::update);
}

void BoxDialog::update()
{
    double xlow = m_boxWidget->xLowLineEdit->text().toDouble();
    double xhigh = m_boxWidget->xHighLineEdit->text().toDouble();
    double ylow = m_boxWidget->yLowLineEdit->text().toDouble();
    double yhigh = m_boxWidget->yHighLineEdit->text().toDouble();
    double zlow = m_boxWidget->zLowLineEdit->text().toDouble();
    double zhigh = m_boxWidget->zHighLineEdit->text().toDouble();

    try {
        m_boxModel->setLimiter(Model::BoxModel::Limiter(xlow, xhigh, ylow, yhigh, zlow, zhigh));
        this->setErrorText(QString{});
    }
    catch (const std::exception& e) {
        this->setErrorText(e.what());
    }
}

void BoxDialog::initialize()
{
    Model::BoxModel::Limiter limiter = m_boxModel->limiter();
    m_boxWidget->xLowLineEdit->setText(QString::number(limiter.xlow));
    m_boxWidget->xHighLineEdit->setText(QString::number(limiter.xhigh));
    m_boxWidget->yLowLineEdit->setText(QString::number(limiter.ylow));
    m_boxWidget->yHighLineEdit->setText(QString::number(limiter.yhigh));
    m_boxWidget->zLowLineEdit->setText(QString::number(limiter.zlow));
    m_boxWidget->zHighLineEdit->setText(QString::number(limiter.zhigh));
}

} // namespace ThanuvaUi
