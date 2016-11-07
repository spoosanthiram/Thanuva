project(Core CXX)

set(SOURCE_FILES
    ${Core_SOURCE_DIR}/Include/Color.h
    ${Core_SOURCE_DIR}/Include/CoreDef.h
    ${Core_SOURCE_DIR}/Include/ThanuvaException.h
    ${Core_SOURCE_DIR}/Include/HVector.h
    ${Core_SOURCE_DIR}/Include/Material.h
    ${Core_SOURCE_DIR}/Include/Matrix3x3.h
    ${Core_SOURCE_DIR}/Include/Matrix4x4.h
    ${Core_SOURCE_DIR}/Include/Quaternion.h
    ${Core_SOURCE_DIR}/Include/StringUtils.h
    ${Core_SOURCE_DIR}/Include/Vector3d.h

    ${Core_SOURCE_DIR}/Source/Color.cpp
    ${Core_SOURCE_DIR}/Source/CoreDef.cpp
    ${Core_SOURCE_DIR}/Source/ThanuvaException.cpp
    ${Core_SOURCE_DIR}/Source/HVector.cpp
    ${Core_SOURCE_DIR}/Source/Material.cpp
    ${Core_SOURCE_DIR}/Source/Matrix3x3.cpp
    ${Core_SOURCE_DIR}/Source/Matrix4x4.cpp
    ${Core_SOURCE_DIR}/Source/Quaternion.cpp
    ${Core_SOURCE_DIR}/Source/StringUtils.cpp
    ${Core_SOURCE_DIR}/Source/Vector3d.cpp
)

include_directories(
    ${Core_SOURCE_DIR}/Include
    ${Thanuva_SOURCE_DIR}/../Algorithms/Include
    ${Thanuva_SOURCE_DIR}/../3rdParty/fmt
)

# TARGET for library
add_library(Core ${SOURCE_FILES})

# TARGET for unit test executable
add_executable(CoreTest ${SOURCE_FILES} ${Core_SOURCE_DIR}/Source/main.cpp)
target_compile_definitions(CoreTest PUBLIC UNIT_TEST)
target_include_directories(CoreTest PUBLIC
    ${Thanuva_SOURCE_DIR}/../3rdParty/googletest/googletest/include
)

if (WIN32)
target_link_libraries(CoreTest
    ${Thanuva_SOURCE_DIR}/../Algorithms/MSVC/x64/Debug/Algo.lib
    ${Thanuva_SOURCE_DIR}/../3rdParty/fmt/build/fmt/Debug/fmt.lib
    ${Thanuva_SOURCE_DIR}/../3rdParty/googletest/googletest/msvc/Debug/gtest.lib
)
else()
target_link_libraries(CoreTest
    ${Thanuva_SOURCE_DIR}/../Algorithms/libAlgo.a
    ${Thanuva_SOURCE_DIR}/../3rdParty/fmt/fmt/libfmt.a
    ${Thanuva_SOURCE_DIR}/../3rdParty/googletest/googletest/libgtest.a
    pthread
    stdc++
)
endif()
