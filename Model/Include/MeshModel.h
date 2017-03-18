/*
 * Model: Model objects for Thanuva
 *
 * Copyright 2016, Saravanan Poosanthiram
 * All rights reserved.
 */

#ifndef MODEL_MESHMODEL_H
#define MODEL_MESHMODEL_H

#include <filesystem>
#include <string>

#include "ModelObject.h"

namespace fs = std::experimental::filesystem;

namespace Model {

class MeshModel : public ModelObject
{
public:
    MeshModel(const Scene* scene) : ModelObject{scene} {}
    MeshModel(const Scene* scene, const fs::path& filePath);
    MeshModel(const MeshModel& rhs) = delete; // TODO: needs to be implemented

    MeshModel& operator=(const MeshModel& rhs) = delete; // TODO: needs to be implemented

    Type type() const override { return Type::Mesh; }
    const fs::path& filePath() const { return m_filePath; }

    void setFilePath(const fs::path& filePath,
                     Core::EmitSignal emitSignal = Core::EmitSignal::Emit);

public: // signals
    Nano::Signal<void()> filePathChanged{};

protected:
    void loadModel(const boost::property_tree::ptree& modelPropTree) override;
    void saveModel(boost::property_tree::ptree& modelPropTree) override;

private:
    fs::path m_filePath{};
};

} // namespace Model

#endif // MODEL_MESHMODEL_H
