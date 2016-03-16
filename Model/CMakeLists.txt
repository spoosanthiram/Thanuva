project(Model CXX)

set(SOURCE_FILES
    ${Model_SOURCE_DIR}/Include/BoxModel.h
    ${Model_SOURCE_DIR}/Include/ModelException.h
    ${Model_SOURCE_DIR}/Include/ModelObject.h
    ${Model_SOURCE_DIR}/Include/Project.h
    ${Model_SOURCE_DIR}/Include/StlModel.h
    ${Model_SOURCE_DIR}/Include/ViewpointCameraModel.h

    ${Model_SOURCE_DIR}/Source/BoxModel.cpp
    ${Model_SOURCE_DIR}/Source/ModelException.cpp
    ${Model_SOURCE_DIR}/Source/ModelObject.cpp
    ${Model_SOURCE_DIR}/Source/Project.cpp
    ${Model_SOURCE_DIR}/Source/StlModel.cpp
    ${Model_SOURCE_DIR}/Source/ViewpointCameraModel.cpp
)

include_directories(
    ${Model_SOURCE_DIR}/Include
    ${Thanuva_SOURCE_DIR}/Core/Include
    ${Thanuva_SOURCE_DIR}/../Algorithms/Include
    ${Thanuva_SOURCE_DIR}/../3rdParty/boost_1_57_0/include
    ${Thanuva_SOURCE_DIR}/../3rdParty/cppformat
    ${Thanuva_SOURCE_DIR}/../3rdParty/glog/src
    ${Thanuva_SOURCE_DIR}/../3rdParty/nano-signal-slot
)

#link_directories(
#    ${Thanuva_SOURCE_DIR}/../3rdParty/boost_1_57_0/lib
#)

# TARGET for library
add_library(Model ${SOURCE_FILES})

#TARGET for unit test executable
add_executable(ModelTest ${SOURCE_FILES} ${Model_SOURCE_DIR}/Source/main.cpp)
target_compile_definitions(ModelTest PUBLIC UNIT_TEST)
target_include_directories(ModelTest PUBLIC
    ${Thanuva_SOURCE_DIR}/../3rdParty/googletest/googletest/include
)
target_link_libraries(ModelTest
    Core
    ${Thanuva_SOURCE_DIR}/../Algorithms/libAlgo.a
    ${Thanuva_SOURCE_DIR}/../3rdParty/cppformat/libcppformat.a
    ${Thanuva_SOURCE_DIR}/../3rdParty/googletest/googletest/libgtest.a
    ${Thanuva_SOURCE_DIR}/../3rdParty/glog/libglog.a
    pthread
    stdc++
)
