/*
 * ThanuvaUi: UI for Thanuva
 *
 * Copyright 2016, Saravanan Poosanthiram
 * All rights reserved.
 */

#include "BoxDialog.h"

#include <QMessageBox>

#include "AppSettings.h"
#include "Box.h"

#include "glog/logging.h"

namespace GlViewer {

BoxDialog::BoxDialog(QWidget* parent, const std::shared_ptr<Model::Box>& box)
    : QDialog{parent/*, Qt::Dialog | Qt::FramelessWindowHint*/}
    , m_box{box}
{
    this->setupUi(this);
    this->setWindowOpacity(AppSettings().windowOpacity());

    m_errorLabel->setText("");

    this->initialize();

    m_box->geometryChanged.connect<BoxDialog, &BoxDialog::initialize>(this);

    connect(m_xLowLineEdit, &QLineEdit::editingFinished, this, &BoxDialog::update);
    connect(m_xHighLineEdit, &QLineEdit::editingFinished, this, &BoxDialog::update);
    connect(m_yLowLineEdit, &QLineEdit::editingFinished, this, &BoxDialog::update);
    connect(m_yHighLineEdit, &QLineEdit::editingFinished, this, &BoxDialog::update);
    connect(m_zLowLineEdit, &QLineEdit::editingFinished, this, &BoxDialog::update);
    connect(m_zHighLineEdit, &QLineEdit::editingFinished, this, &BoxDialog::update);
}

void BoxDialog::update()
{
    double xlow = m_xLowLineEdit->text().toDouble();
    double xhigh = m_xHighLineEdit->text().toDouble();
    double ylow = m_yLowLineEdit->text().toDouble();
    double yhigh = m_yHighLineEdit->text().toDouble();
    double zlow = m_zLowLineEdit->text().toDouble();
    double zhigh = m_zHighLineEdit->text().toDouble();

    LOG(INFO) << "Update the Box limiter with user input values.";
    try {
        m_box->setLimiter(Model::Box::Limiter(xlow, xhigh, ylow, yhigh, zlow, zhigh));
    }
    catch (const std::exception& e) {
        LOG(WARNING) << e.what();
        m_errorLabel->setText(e.what());
    }
}

void BoxDialog::initialize()
{
    Model::Box::Limiter limiter = m_box->limiter();
    m_xLowLineEdit->setText(QString::number(limiter.xlow));
    m_xHighLineEdit->setText(QString::number(limiter.xhigh));
    m_yLowLineEdit->setText(QString::number(limiter.ylow));
    m_yHighLineEdit->setText(QString::number(limiter.yhigh));
    m_zLowLineEdit->setText(QString::number(limiter.zlow));
    m_zHighLineEdit->setText(QString::number(limiter.zhigh));
}

} // namespace GlViewer
