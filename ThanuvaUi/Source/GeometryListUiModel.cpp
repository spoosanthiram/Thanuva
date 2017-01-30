/*
 * ThanuvaUi: UI for Thanuva
 *
 * Copyright 2016, Saravanan Poosanthiram
 * All rights reserved.
 */

#include "GeometryListUiModel.h"

#include "Scene.h"

namespace ThanuvaUi {

int GeometryListUiModel::rowCount(const QModelIndex& /*parent*/) const
{
    return m_scene ? static_cast<int>(m_scene->modelObjectList().size()) : 0;
}

QVariant GeometryListUiModel::data(const QModelIndex& index, int role) const
{
    QVariant retval{};

    if (index.row() < 0 || index.row() >= m_scene->modelObjectList().size())
        return retval;

    auto& mol = m_scene->modelObjectList();
    if (role == Qt::DisplayRole)
        retval.setValue(QString{mol[index.row()]->label().c_str()});

    return retval;
}

void GeometryListUiModel::activate(Model::Scene* scene)
{
    this->beginResetModel(); // let the view know

    m_scene = scene;
    for (auto& moptr : m_scene->modelObjectList())
        moptr->modelObjectChanged.connect<GeometryListUiModel,
                                          &GeometryListUiModel::handleModelObjectChanged>(this);
    m_scene->modelObjectAdded.connect<GeometryListUiModel, &GeometryListUiModel::add>(this);

    this->endResetModel();
}

void GeometryListUiModel::dectivate()
{
    m_scene->modelObjectAdded.disconnect<GeometryListUiModel, &GeometryListUiModel::add>(this);
}

void GeometryListUiModel::handleModelObjectChanged(Model::ModelObject* modelObject)
{
    auto i = m_scene->index(modelObject);
    emit dataChanged(this->createIndex(i, 0), this->createIndex(i, 0));
}

void GeometryListUiModel::add(Model::ModelObject* modelObject)
{
    int newSize = static_cast<int>(m_scene->modelObjectList().size());
    this->beginInsertRows(QModelIndex(), newSize - 1, newSize - 1);
    modelObject->modelObjectChanged.connect<GeometryListUiModel,
                                            &GeometryListUiModel::handleModelObjectChanged>(this);
    this->endInsertRows();
}

} // namespace ThanuvaUi
