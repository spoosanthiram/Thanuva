cmake_minimum_required(VERSION 2.8)

project(Model CXX)

include_directories(
    ${Model_SOURCE_DIR}/Include
    ${Model_SOURCE_DIR}/../Core/Include
    ${Model_SOURCE_DIR}/../../Algorithms/Include
    ${Model_SOURCE_DIR}/../../3rdParty/boost_1_57_0/include
    ${Model_SOURCE_DIR}/../../3rdParty/glog/src
    ${Model_SOURCE_DIR}/../../3rdParty/nano-signal-slot
)

set(SOURCE_FILES
    ${Model_SOURCE_DIR}/Source/Box.cpp
    ${Model_SOURCE_DIR}/Source/Geometry.cpp
    ${Model_SOURCE_DIR}/Source/GeometryException.cpp
    ${Model_SOURCE_DIR}/Source/Project.cpp
    ${Model_SOURCE_DIR}/Source/ProjectException.cpp
    ${Model_SOURCE_DIR}/Source/Stl.cpp
    ${Model_SOURCE_DIR}/Source/ViewpointCameraModel.cpp
)

add_library(Model ${SOURCE_FILES})
