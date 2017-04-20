/*
 * Model: Model objects for Thanuva
 *
 * Copyright 2016, Saravanan Poosanthiram
 * All rights reserved.
 */

#ifndef Model_GeometryModel_h
#define Model_GeometryModel_h

#include "Material.h"
#include "ThanuvaModel.h"

namespace Model {

class CoordinateSystemModel;

class GeometryModel : public ThanuvaModel {
public:
    static const char* kTypeTag;

public:
    GeometryModel(const Scene* scene);

    virtual std::string type() const = 0;
    std::string label() const;
    const CoordinateSystemModel* coordinateSystemModel() const { return m_coordinateSystemModel; }
    const Core::Material& material() const { return m_material; }

    void setCoordinateSystemModel(const CoordinateSystemModel* csys);
    void setMaterial(const Core::Material& material);

public: // signals
    Nano::Signal<void()> coordinateSystemModelChanged{};
    Nano::Signal<void()> materialChanged{};

protected:
    void loadModel(const boost::property_tree::ptree& modelPropTree) override;
    void saveModel(boost::property_tree::ptree& modelPropTree) override;

    virtual void loadGeometryModel(const boost::property_tree::ptree& modelPropTree) = 0;
    virtual void saveGeometryModel(boost::property_tree::ptree& modelPropTree) = 0;

private:
    const CoordinateSystemModel* m_coordinateSystemModel;
    Core::Material m_material{Core::Material::defaultMaterial()};
};

} // namespace Model

#endif // !Model_GeometryModel_h
