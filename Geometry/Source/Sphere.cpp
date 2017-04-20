/*
 * Geometry: Geometry objects for Thanuva
 *
 * Copyright 2016, Saravanan Poosanthiram
 * All rights reserved.
 */

#include "Sphere.h"

#include "SphereModel.h"

namespace Geometry {

Sphere::Sphere(const SceneGeometry* sceneGeometry, Model::SphereModel* sphereModel)
    : Geometry{sceneGeometry, sphereModel}
{
    this->initialize();

    sphereModel->centerChanged.connect<Sphere, &Sphere::initialize>(this);
}

void Sphere::initialize()
{
    this->clear();

    this->setBoundingBox(Extent{std::numeric_limits<double>::infinity(), -std::numeric_limits<double>::infinity(),
                         std::numeric_limits<double>::infinity(), -std::numeric_limits<double>::infinity(),
                         std::numeric_limits<double>::infinity(), -std::numeric_limits<double>::infinity()});

    std::deque<int> indices;
    this->initializeOctahedron(indices);
    //auto sphereModel = dynamic_cast<Model::SphereModel*>(this->modelObject());
    this->subdivide(indices);

    int i0, i1, i2;
    for (auto it = indices.begin(); it != indices.end();) {
        i0 = *it++;
        i1 = *it++;
        i2 = *it++;
        this->insertIndices(i0, i1, i2);
    }

    this->initializeBoundingBox();
    this->updateExtent();

    // emit signals
    geometryChanged.emit_signal();
    extentChanged.emit_signal();
}

void Sphere::initializeOctahedron(std::deque<int>& indices)
{
    // bottom vertex
    Core::Point3d v0{0.0, 0.0, -1.0};
    this->insertVertex(v0);
    this->insertNormal(v0.data());

    // middle vertices
    Core::Point3d v1{1.0, 0.0, 0.0};
    this->insertVertex(v1);
    this->insertNormal(v1.data());
    Core::Point3d v2{0.0, 1.0, 0.0};
    this->insertVertex(v2);
    this->insertNormal(v2.data());
    Core::Point3d v3{-1.0, 0.0, 0.0};
    this->insertVertex(v3);
    this->insertNormal(v3.data());
    Core::Point3d v4{0.0, -1.0, 0.0};
    this->insertVertex(v4);
    this->insertNormal(v4.data());

    // top vertex
    Core::Point3d v5{0.0, 0.0, 1.0};
    this->insertVertex(v5);
    this->insertNormal(v5.data());

    // Bottom Facets
    indices.push_back(0); indices.push_back(2); indices.push_back(1);
    indices.push_back(0); indices.push_back(3); indices.push_back(2);
    indices.push_back(0); indices.push_back(4); indices.push_back(3);
    indices.push_back(0); indices.push_back(1); indices.push_back(4);

    // Top Facets
    indices.push_back(1); indices.push_back(2); indices.push_back(5);
    indices.push_back(2); indices.push_back(3); indices.push_back(5);
    indices.push_back(3); indices.push_back(4); indices.push_back(5);
    indices.push_back(4); indices.push_back(1); indices.push_back(5);
}

void Sphere::subdivide(std::deque<int>& indices)
{
    std::map<int64_t, int> midVertexIndexMap{};

    int nsub = 4;
    while (nsub-- > 0) {

        int i0, i1, i2;
        int i3, i4, i5;

        int size = indices.size();

        for (int i = 0; i < size; i += 3) {
            i0 = indices.front(); indices.pop_front();
            i1 = indices.front(); indices.pop_front();
            i2 = indices.front(); indices.pop_front();

            i3 = this->midVertexIndex(i0, i1, midVertexIndexMap);
            i4 = this->midVertexIndex(i1, i2, midVertexIndexMap);
            i5 = this->midVertexIndex(i0, i2, midVertexIndexMap);

            indices.push_back(i0); indices.push_back(i3); indices.push_back(i5);
            indices.push_back(i3); indices.push_back(i1); indices.push_back(i4);
            indices.push_back(i5); indices.push_back(i3); indices.push_back(i4);
            indices.push_back(i5); indices.push_back(i4); indices.push_back(i2);
        }
    }
}

int Sphere::midVertexIndex(int i0, int i1, std::map<int64_t, int>&midVertexIndexMap)
{
    int64_t key = 0;
    if (i0 < i1) {
        key = i0;
        key = (key << (sizeof(int) * 8)) | i1;
    }
    else {
        key = i1;
        key = (key << (sizeof(int) * 8)) | i0;
    }

    int midIndex;

    auto it = midVertexIndexMap.find(key);
    if (it == midVertexIndexMap.end()) {
        auto& vertices = this->vertices();
        Core::Vector3d v0{&vertices[i0 * Geometry::kValuesPerVertex]};
        Core::Vector3d v1{&vertices[i1 * Geometry::kValuesPerVertex]};

        Core::Vector3d mid = v0 + v1;
        mid.normalize();

        midIndex = vertices.size() / Geometry::kValuesPerVertex;

        this->insertVertex(mid.data());
        this->insertNormal(mid);

        midVertexIndexMap[key] = midIndex;
    }
    else {
        midIndex = it->second;
        midVertexIndexMap.erase(it);
    }

    return midIndex;
}

} // namespace Geometry
