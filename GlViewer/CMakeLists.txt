cmake_minimum_required (VERSION 2.8.11)

project(GlViewer CXX)

add_definitions(-DGLEW_STATIC)

set(CMAKE_INCLUDE_CURRENT_DIR ON) # to inclue moc and ui files
#set(CMAKE_AUTOMOC ON)

find_package(Qt5OpenGL)
find_package(Qt5Widgets)

include_directories(
    ${GlViewer_SOURCE_DIR}/Include
    ${GlViewer_SOURCE_DIR}/..
    ${GlViewer_SOURCE_DIR}/../Core/Include
    ${GlViewer_SOURCE_DIR}/../Model/Include
    ${GlViewer_SOURCE_DIR}/../GfxModel/Include
    ${GlViewer_SOURCE_DIR}/../../Algorithms/Include
    ${GlViewer_SOURCE_DIR}/../../3rdParty/nano-signal-slot
    ${GlViewer_SOURCE_DIR}/../../3rdParty/google-glog/include
    ${GlViewer_SOURCE_DIR}/../../3rdParty/glew/include
)

set(SOURCE_FILES
    ${GlViewer_SOURCE_DIR}/Source/AppSettings.cpp
    ${GlViewer_SOURCE_DIR}/Source/BoxDialog.cpp
    ${GlViewer_SOURCE_DIR}/Source/GlProject.cpp
    ${GlViewer_SOURCE_DIR}/Source/GlViewerException.cpp
    ${GlViewer_SOURCE_DIR}/Source/GlViewObject.cpp
    ${GlViewer_SOURCE_DIR}/Source/GlWidget.cpp
    ${GlViewer_SOURCE_DIR}/Source/main.cpp
    ${GlViewer_SOURCE_DIR}/Source/MainWindow.cpp
    ${GlViewer_SOURCE_DIR}/Source/Shader.cpp
    ${GlViewer_SOURCE_DIR}/Source/ShaderProgram.cpp
)

qt5_wrap_ui(UI_FILES
    Form/BoxDialog.ui
)

qt5_wrap_cpp(MOC_FILES
    ${GlViewer_SOURCE_DIR}/Include/BoxDialog.h
    ${GlViewer_SOURCE_DIR}/Include/GlProject.h
    ${GlViewer_SOURCE_DIR}/Include/GlWidget.h
)

link_directories(
    ${GlViewer_SOURCE_DIR}/../../Algorithms
    ${GlViewer_SOURCE_DIR}/../../3rdParty/boost_1_57_0/lib
    ${GlViewer_SOURCE_DIR}/../../3rdParty/google-glog/lib
    ${GlViewer_SOURCE_DIR}/../../3rdParty/glew/lib
)

add_executable(GlViewer ${SOURCE_FILES} ${UI_FILES} ${MOC_FILES})
target_link_libraries(GlViewer GfxModel Model Core Algo glog GLEW Qt5::Widgets Qt5::OpenGL GLU stdc++)
