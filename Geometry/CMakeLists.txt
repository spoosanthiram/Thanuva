project(Geometry CXX)

if(WIN32)
    add_definitions(-DGOOGLE_GLOG_DLL_DECL=)
endif()

add_definitions(-DGLOG_NO_ABBREVIATED_SEVERITIES)

set(SOURCE_FILES
    ${Geometry_SOURCE_DIR}/Include/Box.h
    ${Geometry_SOURCE_DIR}/Include/Cone.h
    ${Geometry_SOURCE_DIR}/Include/Cylinder.h
    ${Geometry_SOURCE_DIR}/Include/Extent.h
    ${Geometry_SOURCE_DIR}/Include/GeometryCommon.h
    ${Geometry_SOURCE_DIR}/Include/GeometryContainer.h
    ${Geometry_SOURCE_DIR}/Include/GeometryObject.h
    ${Geometry_SOURCE_DIR}/Include/Mesh.h
    ${Geometry_SOURCE_DIR}/Include/MeshReader.h
    ${Geometry_SOURCE_DIR}/Include/StlMeshReader.h

    ${Geometry_SOURCE_DIR}/Source/Box.cpp
    ${Geometry_SOURCE_DIR}/Source/Cone.cpp
    ${Geometry_SOURCE_DIR}/Source/Cylinder.cpp
    ${Geometry_SOURCE_DIR}/Source/Extent.cpp
    ${Geometry_SOURCE_DIR}/Source/GeometryCommon.cpp
    ${Geometry_SOURCE_DIR}/Source/GeometryContainer.cpp
    ${Geometry_SOURCE_DIR}/Source/GeometryObject.cpp
    ${Geometry_SOURCE_DIR}/Source/Mesh.cpp
    ${Geometry_SOURCE_DIR}/Source/MeshReader.cpp
    ${Geometry_SOURCE_DIR}/Source/StlMeshReader.cpp
)

include_directories(
    ${Geometry_SOURCE_DIR}/Include
    ${Thanuva_SOURCE_DIR}/Core/Include
    ${Thanuva_SOURCE_DIR}/Model/Include
    ${Thanuva_SOURCE_DIR}/../Algorithms/Include
    ${Thanuva_SOURCE_DIR}/../3rdParty/boost
    ${Thanuva_SOURCE_DIR}/../3rdParty/glog/src/windows
    ${Thanuva_SOURCE_DIR}/../3rdParty/nano-signal-slot
)

add_library(Geometry ${SOURCE_FILES})
