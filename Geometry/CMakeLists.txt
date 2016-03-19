project(Geometry CXX)

set(SOURCE_FILES
    ${Geometry_SOURCE_DIR}/Include/Box.h
    ${Geometry_SOURCE_DIR}/Include/Extent.h
    ${Geometry_SOURCE_DIR}/Include/GeometryContainer.h
    ${Geometry_SOURCE_DIR}/Include/GeometryObject.h
    ${Geometry_SOURCE_DIR}/Include/Stl.h

    ${Geometry_SOURCE_DIR}/Source/Box.cpp
    ${Geometry_SOURCE_DIR}/Source/Extent.cpp
    ${Geometry_SOURCE_DIR}/Source/GeometryContainer.cpp
    ${Geometry_SOURCE_DIR}/Source/GeometryObject.cpp
    ${Geometry_SOURCE_DIR}/Source/Stl.cpp
)

include_directories(
    ${Geometry_SOURCE_DIR}/Include
    ${Thanuva_SOURCE_DIR}/Core/Include
    ${Thanuva_SOURCE_DIR}/Model/Include
    ${Thanuva_SOURCE_DIR}/../Algorithms/Include
    ${Thanuva_SOURCE_DIR}/../3rdParty/glog/src
    ${Thanuva_SOURCE_DIR}/../3rdParty/nano-signal-slot
)

add_library(Geometry ${SOURCE_FILES})
