/*
 * Graphics: Graphical objects for Thanuva Model/Geometry obejcts
 *
 * Copyright 2016, Saravanan Poosanthiram
 * All rights reserved.
 */

#ifndef Graphics_AxisLegendScene_h
#define Graphics_AxisLegendScene_h

#include "AbstractGraphicsScene.h"

namespace Model {
    class Scene;
    class ThanuvaApp;
}

namespace Geometry { class GeometryScene; }

namespace Graphics {

class AxisLegendScene : public AbstractGraphicsScene
{
public:
    static const double kLegendFarProjection;

public:
    AxisLegendScene(const Model::ThanuvaApp& app);

    GLint diffuseColorLocation() const override { return m_diffuseColorLocation; }
    bool isSpecularColorAvailable() const override { return false; }
    GLint specularColorLocation() const override { return -1; }
    GLint shininessLocation() const override { return -1; }

    void activateScene();

    std::string vertexShaderSource() override;
    std::string fragmentShaderSource() override;
    void updateShaderLocations() override;
    void handleWindowAspectChanged() override;
    void render() const override;

private:
    std::unique_ptr<Model::Scene> m_scene;
    std::unique_ptr<Geometry::GeometryScene> m_geometryScene{};

    std::array<float, 4> m_light0Position{{0.0f, 0.0f, 1.0f, 1.0f}};
    std::array<float, 4> m_light0DiffuseColor{{0.7f, 0.7f, 0.7f, 1.0f}};

    GLint m_light0PositionLocation{-1};
    GLint m_light0DiffuseColorLocation{-1};

    GLint m_diffuseColorLocation{-1};
};

} // namespace Graphics

#endif // Graphics_AxisLegendScene_h
