/**
* Author: Saravanan Poosanthiram
* $LastChangedBy: ps $
* $LastChangedDate: 2015-03-20 18:08:01 -0400 (Fri, 20 Mar 2015) $
*/

#include "Stl.h"

#include <boost/property_tree/ptree.hpp>

namespace {

const char* kFilePathTag = "filePath";

} // anonymous

namespace Model {

Stl::Stl(const Project& project, const std::string& filePath)
    : Geometry{project}
#ifndef _MSC_VER
    , m_filePath{filePath}
#else
    , m_filePath(filePath)
#endif
{
    // TODO: check if STL is exists
}

void Stl::setFilePath(const std::string& filePath, Core::EmitSignal emitSignal)
{
    if (filePath == m_filePath)
        return;

    // TODO: check if STL is exists
    m_filePath = filePath;

    if (Core::EmitSignal::Emit == emitSignal)
        geometryChanged(); // emit signal
}

void Stl::load(const boost::property_tree::ptree& geometryPropTree)
{
    this->loadGeometry(geometryPropTree);

    m_filePath = geometryPropTree.get<std::string>(kFilePathTag);
}

void Stl::save(boost::property_tree::ptree& geometryPropTree)
{
    this->saveGeometry(geometryPropTree);

    geometryPropTree.put(kFilePathTag, m_filePath);
}

} // namespace Model
