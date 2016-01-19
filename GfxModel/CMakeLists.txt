cmake_minimum_required(VERSION 2.8)

project(GfxModel CXX)

include_directories(
    ${GfxModel_SOURCE_DIR}/Include
    ${GfxModel_SOURCE_DIR}/../Core/Include
    ${GfxModel_SOURCE_DIR}/../Model/Include
    ${GfxModel_SOURCE_DIR}/../../Algorithms/Include
    ${GfxModel_SOURCE_DIR}/../../3rdParty/glog/src
    ${GfxModel_SOURCE_DIR}/../../3rdParty/nano-signal-slot
)

set(SOURCE_FILES
    ${GfxModel_SOURCE_DIR}/Source/Extent.cpp
    ${GfxModel_SOURCE_DIR}/Source/GfxBox.cpp
    ${GfxModel_SOURCE_DIR}/Source/GfxProject.cpp
    ${GfxModel_SOURCE_DIR}/Source/GfxStl.cpp
    ${GfxModel_SOURCE_DIR}/Source/GraphicsObject.cpp
    ${GfxModel_SOURCE_DIR}/Source/ViewpointCamera.cpp
)

add_library(GfxModel ${SOURCE_FILES})
