/*
 * Model: Model objects for Thanuva
 *
 * Copyright 2016, Saravanan Poosanthiram
 * All rights reserved.
 */

#ifndef MODEL_STL_H
#define MODEL_STL_H

#include <cstdint>
#include <string>

#include "ModelObject.h"

namespace Model {

class Stl : public ModelObject
{
public:
    Stl(const Project& project) : ModelObject{project} {}
    Stl(const Project& project, const std::string& filePath);
    Stl(const Stl& rhs) = delete; // TODO: needs to be implemented

    Stl& operator=(const Stl& rhs) = delete; // TODO: needs to be implemented

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

#endif // MODEL_STL_H
