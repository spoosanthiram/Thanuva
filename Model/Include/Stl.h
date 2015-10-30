/**
* Author: Saravanan Poosanthiram
* $LastChangedBy: ps $
* $LastChangedDate: 2015-03-19 18:15:00 -0400 (Thu, 19 Mar 2015) $
*/

#ifndef MODEL_STL_H
#define MODEL_STL_H

#include <cstdint>
#include <string>

#include "Geometry.h"

namespace Model {

class Stl : public Geometry
{
public:
    Stl(const Project& project) : Geometry{project}, m_filePath{} {}
    Stl(const Project& project, const std::string& filePath);
    Stl(const Stl& rhs) = delete; // TODO: needs to be implemented

    Stl& operator=(const Stl& rhs) = delete; // TODO: needs to be implemented

    Type type() const override { return Type::Stl; }
    const std::string& filePath() const { return m_filePath; }

    void setFilePath(const std::string& filePath, Core::EmitSignal emitSignal = Core::EmitSignal::Emit);

    void load(const boost::property_tree::ptree& geometryPropTree) override;
    void save(boost::property_tree::ptree& geometryPropTree) override;

private:
    std::string m_filePath;
};

} // namespace Model

#endif // MODEL_STL_H
