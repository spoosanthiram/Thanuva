project(Model CXX)

if(WIN32)
    add_definitions(-DGOOGLE_GLOG_DLL_DECL=)
endif()

add_definitions(-DGLOG_NO_ABBREVIATED_SEVERITIES)

set(SOURCE_FILES
    ${Model_SOURCE_DIR}/Include/BoxModel.h
    ${Model_SOURCE_DIR}/Include/ModelException.h
    ${Model_SOURCE_DIR}/Include/ModelObject.h
    ${Model_SOURCE_DIR}/Include/Project.h
    ${Model_SOURCE_DIR}/Include/StlModel.h
    ${Model_SOURCE_DIR}/Include/Viewpoint.h

    ${Model_SOURCE_DIR}/Source/BoxModel.cpp
    ${Model_SOURCE_DIR}/Source/ModelException.cpp
    ${Model_SOURCE_DIR}/Source/ModelObject.cpp
    ${Model_SOURCE_DIR}/Source/Project.cpp
    ${Model_SOURCE_DIR}/Source/StlModel.cpp
    ${Model_SOURCE_DIR}/Source/Viewpoint.cpp
)

include_directories(
    ${Model_SOURCE_DIR}/Include
    ${Thanuva_SOURCE_DIR}/Core/Include
    ${Thanuva_SOURCE_DIR}/../Algorithms/Include
    ${Thanuva_SOURCE_DIR}/../3rdParty/boost
    ${Thanuva_SOURCE_DIR}/../3rdParty/fmt
    ${Thanuva_SOURCE_DIR}/../3rdParty/glog/src/windows
    ${Thanuva_SOURCE_DIR}/../3rdParty/nano-signal-slot
)

# TARGET for library
add_library(Model ${SOURCE_FILES})

#TARGET for unit test executable
add_executable(ModelTest ${SOURCE_FILES} ${Model_SOURCE_DIR}/Source/main.cpp)
target_compile_definitions(ModelTest PUBLIC UNIT_TEST)
target_include_directories(ModelTest PUBLIC
    ${Thanuva_SOURCE_DIR}/../3rdParty/googletest/googletest/include
)
if(WIN32)
target_link_libraries(ModelTest
    Core
    ${Thanuva_SOURCE_DIR}/../Algorithms/MSVC/x64/Debug/Algo.lib
    ${Thanuva_SOURCE_DIR}/../3rdParty/fmt/build/fmt/Debug/fmt.lib
    ${Thanuva_SOURCE_DIR}/../3rdParty/glog/x64/Debug/libglog_static.lib
    ${Thanuva_SOURCE_DIR}/../3rdParty/googletest/googletest/msvc/Debug/gtest.lib
)
else()
target_link_libraries(ModelTest
    Core
    ${Thanuva_SOURCE_DIR}/../Algorithms/libAlgo.a
    ${Thanuva_SOURCE_DIR}/../3rdParty/fmt/fmt/libfmt.a
    ${Thanuva_SOURCE_DIR}/../3rdParty/glog/libglog.a
    ${Thanuva_SOURCE_DIR}/../3rdParty/googletest/googletest/libgtest.a
    pthread
    stdc++
)
endif()
