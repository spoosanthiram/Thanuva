/*
 * Graphics: Graphical objects for Thanuva Model/Geometry obejcts
 *
 * Copyright 2016, Saravanan Poosanthiram
 * All rights reserved.
 */

#ifndef Graphics_GraphicsScene_h
#define Graphics_GraphicsScene_h

#include "AbstractGraphicsScene.h"
#include "GeometryScene.h"

namespace Graphics {

class GraphicsScene : public AbstractGraphicsScene
{
public:
    static const double kViewpointTranslationMultiplier;
    static const double kFarProjectionMultiplier;

public:
    GraphicsScene();

    const std::unique_ptr<Geometry::GeometryScene>& geometryScene() const { return m_geometryScene; }
    GLint diffuseColorLocation() const override { return m_diffuseColorLocation; }
    bool isSpecularColorAvailable() const override { return true; }
    GLint specularColorLocation() const override { return m_specularColorLocation; }
    GLint shininessLocation() const override { return m_shininessLocation; }

    void activate(Model::Scene* scene);
    void deactivate();
    std::vector<const Graphics::GraphicsObject*> probe(int x, int y) const;

    std::string vertexShaderSource() override;
    std::string fragmentShaderSource() override;
    void updateShaderLocations() override;
    void handleWindowAspectChanged() override { this->updateProjectionMatrix(); }
    void render() const override;

private: // slots
    void handleExtentChanged();

private:
    void updateProjectionMatrix();

    std::unique_ptr<Geometry::GeometryScene> m_geometryScene{};

    std::array<float, 4> m_light0Position{{0.0f, 0.0f, 1.0f, 1.0f}};
    std::array<float, 4> m_light0DiffuseColor{{0.7f, 0.7f, 0.7f, 1.0f}};
    std::array<float, 4> m_light0SpecularColor{{0.4f, 0.4f, 0.4f, 1.0f}};

    GLint m_light0PositionLocation{-1};
    GLint m_light0DiffuseColorLocation{-1};
    GLint m_light0SpecularColorLocation{-1};

    GLint m_diffuseColorLocation{-1};
    GLint m_specularColorLocation{-1};
    GLint m_shininessLocation{-1};
};

} // namespace Graphics

#endif // Graphics_GraphicsScene_h
