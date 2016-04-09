project(Graphics CXX)

set(CMAKE_PREFIX_PATH $ENV{QTDIR})
set(CMAKE_INCLUDE_CURRENT_DIR ON) # to inclue moc files
set(CMAKE_AUTOMOC ON)

find_package(Qt5Core)
find_package(Qt5Gui)

add_definitions(-DGLOG_NO_ABBREVIATED_SEVERITIES)

set(SOURCE_FILES
    ${Graphics_SOURCE_DIR}/Include/GraphicsEnvironment.h
    ${Graphics_SOURCE_DIR}/Include/GraphicsException.h
    ${Graphics_SOURCE_DIR}/Include/GraphicsObject.h
    ${Graphics_SOURCE_DIR}/Include/OpenGLInterface.h
    ${Graphics_SOURCE_DIR}/Include/Shader.h
    ${Graphics_SOURCE_DIR}/Include/ShaderProgram.h
    ${Graphics_SOURCE_DIR}/Include/ViewpointCamera.h

    ${Graphics_SOURCE_DIR}/Source/GraphicsEnvironment.cpp
    ${Graphics_SOURCE_DIR}/Source/GraphicsException.cpp
    ${Graphics_SOURCE_DIR}/Source/GraphicsObject.cpp
    ${Graphics_SOURCE_DIR}/Source/OpenGLInterface.cpp
    ${Graphics_SOURCE_DIR}/Source/Shader.cpp
    ${Graphics_SOURCE_DIR}/Source/ShaderProgram.cpp
    ${Graphics_SOURCE_DIR}/Source/ViewpointCamera.cpp
)

include_directories(
    ${Graphics_SOURCE_DIR}/Include
    ${Thanuva_SOURCE_DIR}/Core/Include
    ${Thanuva_SOURCE_DIR}/Model/Include
    ${Thanuva_SOURCE_DIR}/Geometry/Include
    ${Thanuva_SOURCE_DIR}/../Algorithms/Include
    ${Thanuva_SOURCE_DIR}/../3rdParty/boost
    ${Thanuva_SOURCE_DIR}/../3rdParty/glog/src/windows
    ${Thanuva_SOURCE_DIR}/../3rdParty/nano-signal-slot
)

add_library(Graphics ${SOURCE_FILES})
target_link_libraries(Graphics Qt5::Gui Qt5::Core) # just so that cmake sets up header files
