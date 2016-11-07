/*
 * Model: Model objects for Thanuva
 *
 * Copyright 2016, Saravanan Poosanthiram
 * All rights reserved.
 */

#ifndef MODEL_STLMODEL_H
#define MODEL_STLMODEL_H

#include <string>

#include "ModelObject.h"

namespace Model {

class StlModel : public ModelObject
{
public:
    StlModel(const Scene& scene) : ModelObject{scene} {}
    StlModel(const Scene& scene, const std::string& filePath);
    StlModel(const StlModel& rhs) = delete; // TODO: needs to be implemented

    StlModel& operator=(const StlModel& rhs) = delete; // TODO: needs to be implemented

    Type type() const override { return Type::Stl; }
    const std::string& filePath() const { return m_filePath; }

    void setFilePath(const std::string& filePath,
                     Core::EmitSignal emitSignal = Core::EmitSignal::Emit);

protected:
    void loadModel(const boost::property_tree::ptree& modelPropTree) override;
    void saveModel(boost::property_tree::ptree& modelPropTree) override;

private:
    std::string m_filePath{};
};

} // namespace Model

#endif // MODEL_STLMODEL_H
