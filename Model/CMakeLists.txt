project(Model CXX)

set(SOURCE_FILES
    ${Model_SOURCE_DIR}/Include/Box.h
    ${Model_SOURCE_DIR}/Include/GeometryException.h
    ${Model_SOURCE_DIR}/Include/Geometry.h
    ${Model_SOURCE_DIR}/Include/ProjectException.h
    ${Model_SOURCE_DIR}/Include/Project.h
    ${Model_SOURCE_DIR}/Include/Stl.h
    ${Model_SOURCE_DIR}/Include/ViewpointCameraModel.h

    ${Model_SOURCE_DIR}/Source/Box.cpp
    ${Model_SOURCE_DIR}/Source/Geometry.cpp
    ${Model_SOURCE_DIR}/Source/GeometryException.cpp
    ${Model_SOURCE_DIR}/Source/Project.cpp
    ${Model_SOURCE_DIR}/Source/ProjectException.cpp
    ${Model_SOURCE_DIR}/Source/Stl.cpp
    ${Model_SOURCE_DIR}/Source/ViewpointCameraModel.cpp
)

include_directories(
    ${Model_SOURCE_DIR}/Include
    ${Graphics_SOURCE_DIR}/Core/Include
    ${Graphics_SOURCE_DIR}/../Algorithms/Include
    ${Graphics_SOURCE_DIR}/../3rdParty/boost_1_57_0/include
    ${Graphics_SOURCE_DIR}/../3rdParty/glog/src
    ${Graphics_SOURCE_DIR}/../3rdParty/nano-signal-slot
)

link_directories(
    ${Graphics_SOURCE_DIR}/../Algorithms
    ${Graphics_SOURCE_DIR}/../3rdParty/boost_1_57_0/lib
    ${Graphics_SOURCE_DIR}/../3rdParty/glog
    ${Graphics_SOURCE_DIR}/../3rdParty/googletest/googletest
)

# TARGET for library
add_library(Model ${SOURCE_FILES})

#TARGET for unit test executable
add_executable(ModelTest ${SOURCE_FILES} ${Model_SOURCE_DIR}/Source/main.cpp)
target_compile_definitions(ModelTest PUBLIC UNIT_TEST)
target_include_directories(ModelTest PUBLIC
    ${Graphics_SOURCE_DIR}/../3rdParty/googletest/googletest/include
)
target_link_libraries(ModelTest Core Algo gtest glog pthread stdc++)
