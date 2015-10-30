cmake_minimum_required(VERSION 2.8)

project(Core CXX)

include_directories(
    ${Core_SOURCE_DIR}/Include
    ${Core_SOURCE_DIR}/../../Algorithms/Include
)

set(SOURCE_FILES
    ${Core_SOURCE_DIR}/Source/Color.cpp
    ${Core_SOURCE_DIR}/Source/CoreDef.cpp
    ${Core_SOURCE_DIR}/Source/HVector.cpp
    ${Core_SOURCE_DIR}/Source/Material.cpp
    ${Core_SOURCE_DIR}/Source/Matrix3x3.cpp
    ${Core_SOURCE_DIR}/Source/Matrix4x4.cpp
    ${Core_SOURCE_DIR}/Source/Quaternion.cpp
    ${Core_SOURCE_DIR}/Source/Vector3d.cpp
)

add_library(Core ${SOURCE_FILES})
