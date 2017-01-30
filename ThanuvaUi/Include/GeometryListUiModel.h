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
    class ModelObject;
    class Scene;
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
    void handleModelObjectChanged(Model::ModelObject* modelObject);
    void add(Model::ModelObject* modelObject);

private:
    Model::Scene* m_scene{nullptr};
};

} // namespace ThanuvaUi

#endif // THANUVAUI_GEOMETRYLISTUIMODEL_H
