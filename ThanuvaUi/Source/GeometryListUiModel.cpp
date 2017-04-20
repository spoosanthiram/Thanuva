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
    return m_scene ? static_cast<int>(m_scene->geometryModelList().size()) : 0;
}

QVariant GeometryListUiModel::data(const QModelIndex& index, int role) const
{
    QVariant retval{};

    if (index.row() < 0 || index.row() >= m_scene->geometryModelList().size())
        return retval;

    auto& gml = m_scene->geometryModelList();
    if (role == Qt::DisplayRole)
        retval.setValue(QString{gml[index.row()]->label().c_str()});

    return retval;
}

void GeometryListUiModel::activate(Model::Scene* scene)
{
    this->beginResetModel(); // let the view know

    m_scene = scene;
    for (auto& moptr : m_scene->geometryModelList())
        moptr->thanuvaModelChanged.connect<GeometryListUiModel,
                                          &GeometryListUiModel::handleGeometryModelChanged>(this);
    m_scene->geometryModelAdded.connect<GeometryListUiModel, &GeometryListUiModel::add>(this);

    this->endResetModel();
}

void GeometryListUiModel::dectivate()
{
    m_scene->geometryModelAdded.disconnect<GeometryListUiModel, &GeometryListUiModel::add>(this);
}

void GeometryListUiModel::handleGeometryModelChanged(Model::ThanuvaModel* thanuvaModel)
{
    auto i = m_scene->geometryModelIndex(dynamic_cast<Model::GeometryModel*>(thanuvaModel));
    emit dataChanged(this->createIndex(i, 0), this->createIndex(i, 0));
}

void GeometryListUiModel::add(Model::GeometryModel* geometryModel)
{
    int newSize = static_cast<int>(m_scene->geometryModelList().size());
    this->beginInsertRows(QModelIndex(), newSize - 1, newSize - 1);
    geometryModel->thanuvaModelChanged.connect<GeometryListUiModel,
                                            &GeometryListUiModel::handleGeometryModelChanged>(this);
    this->endInsertRows();
}

} // namespace ThanuvaUi
