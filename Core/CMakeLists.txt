project(Core CXX)

set(SOURCE_FILES
    ${Core_SOURCE_DIR}/Include/Color.h
    ${Core_SOURCE_DIR}/Include/CoreDef.h
    ${Core_SOURCE_DIR}/Include/GraphicsException.h
    ${Core_SOURCE_DIR}/Include/HVector.h
    ${Core_SOURCE_DIR}/Include/Material.h
    ${Core_SOURCE_DIR}/Include/Matrix3x3.h
    ${Core_SOURCE_DIR}/Include/Matrix4x4.h
    ${Core_SOURCE_DIR}/Include/Quaternion.h
    ${Core_SOURCE_DIR}/Include/Vector3d.h

    ${Core_SOURCE_DIR}/Source/Color.cpp
    ${Core_SOURCE_DIR}/Source/CoreDef.cpp
    ${Core_SOURCE_DIR}/Source/HVector.cpp
    ${Core_SOURCE_DIR}/Source/Material.cpp
    ${Core_SOURCE_DIR}/Source/Matrix3x3.cpp
    ${Core_SOURCE_DIR}/Source/Matrix4x4.cpp
    ${Core_SOURCE_DIR}/Source/Quaternion.cpp
    ${Core_SOURCE_DIR}/Source/Vector3d.cpp
)

include_directories(
    ${Core_SOURCE_DIR}/Include
    ${Graphics_SOURCE_DIR}/../Algorithms/Include
)

# TARGET for library
add_library(Core ${SOURCE_FILES})

# TARGET for unit test executable
add_executable(CoreTest ${SOURCE_FILES} ${Core_SOURCE_DIR}/Source/main.cpp)
target_compile_definitions(CoreTest PUBLIC UNIT_TEST)
target_include_directories(CoreTest PUBLIC
    ${Graphics_SOURCE_DIR}/../3rdParty/googletest/googletest/include
)
target_link_libraries(CoreTest
    ${Graphics_SOURCE_DIR}/../Algorithms/libAlgo.a
    ${Graphics_SOURCE_DIR}/../3rdParty/googletest/googletest/libgtest.a
    pthread
    stdc++
)
