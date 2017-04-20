/*
 * ThanuvaUi: UI for Thanuva
 *
 * Copyright 2016, Saravanan Poosanthiram
 * All rights reserved.
 */

#ifndef THANUVAUI_GEOMETRYLISTUIMODEL_H
#define THANUVAUI_GEOMETRYLISTUIMODEL_H

#include <QAbstractListModel>

namespace Model {
    class Scene;
    class GeometryModel;
    class ThanuvaModel;
}

namespace ThanuvaUi {

class GeometryListUiModel : public QAbstractListModel
{
public:
    GeometryListUiModel(QObject* parent) : QAbstractListModel{parent} {}

    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role) const override;

    void activate(Model::Scene* scene);
    void dectivate();

private: // slots
    void handleGeometryModelChanged(Model::ThanuvaModel* thanuvaModel);
    void add(Model::GeometryModel* geometryModel);

private:
    Model::Scene* m_scene{nullptr};
};

} // namespace ThanuvaUi

#endif // THANUVAUI_GEOMETRYLISTUIMODEL_H
