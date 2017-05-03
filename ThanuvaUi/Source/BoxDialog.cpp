/*
 * ThanuvaUi: UI for Thanuva
 *
 * Copyright 2016, Saravanan Poosanthiram
 * All rights reserved.
 */

#include "BoxDialog.h"

#include "BoxModel.h"

namespace ThanuvaUi {

BoxDialog::BoxDialog(QWidget* parent, Model::BoxModel* boxModel)
    : GeometryDialog{parent, boxModel}
    , m_boxModel{boxModel}
{
    this->setupUi(this);
    this->initialize(); // initialize GeometryDialog

    this->updateUiLimiter();

    m_boxModel->limiterChanged.connect<BoxDialog, &BoxDialog::updateUiLimiter>(this);

    connect(m_xLowLineEdit, &QLineEdit::editingFinished, this, &BoxDialog::updateModelLimiter);
    connect(m_xHighLineEdit, &QLineEdit::editingFinished, this, &BoxDialog::updateModelLimiter);
    connect(m_yLowLineEdit, &QLineEdit::editingFinished, this, &BoxDialog::updateModelLimiter);
    connect(m_yHighLineEdit, &QLineEdit::editingFinished, this, &BoxDialog::updateModelLimiter);
    connect(m_zLowLineEdit, &QLineEdit::editingFinished, this, &BoxDialog::updateModelLimiter);
    connect(m_zHighLineEdit, &QLineEdit::editingFinished, this, &BoxDialog::updateModelLimiter);
}

void BoxDialog::updateModelLimiter()
{
    double xlow = m_xLowLineEdit->text().toDouble();
    double xhigh = m_xHighLineEdit->text().toDouble();
    double ylow = m_yLowLineEdit->text().toDouble();
    double yhigh = m_yHighLineEdit->text().toDouble();
    double zlow = m_zLowLineEdit->text().toDouble();
    double zhigh = m_zHighLineEdit->text().toDouble();

    try {
        m_boxModel->setLimiter(Model::BoxModel::Limiter(xlow, xhigh, ylow, yhigh, zlow, zhigh));
        this->setErrorText(QString{});
    }
    catch (const std::exception& e) {
        this->setErrorText(e.what());
    }
}

void BoxDialog::updateUiLimiter()
{
    Model::BoxModel::Limiter limiter = m_boxModel->limiter();

    m_xLowLineEdit->setText(QString::number(limiter.xlow));
    m_xHighLineEdit->setText(QString::number(limiter.xhigh));
    m_yLowLineEdit->setText(QString::number(limiter.ylow));
    m_yHighLineEdit->setText(QString::number(limiter.yhigh));
    m_zLowLineEdit->setText(QString::number(limiter.zlow));
    m_zHighLineEdit->setText(QString::number(limiter.zhigh));
}

} // namespace ThanuvaUi
