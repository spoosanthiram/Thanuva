project(GlViewer CXX)

set(CMAKE_PREFIX_PATH $ENV{QTDIR})

set(CMAKE_INCLUDE_CURRENT_DIR ON) # to inclue moc and ui files
set(CMAKE_AUTOMOC ON)
#set(CMAKE_AUTOUIC ON)
#set(AUTOUIC_SEARCH_PATH ${GlViewer_SOURCE_DIR}/Form)

find_package(Qt5Core)
find_package(Qt5Gui)
find_package(Qt5Widgets)

include_directories(
    ${GlViewer_SOURCE_DIR}/Include
    ${GlViewer_SOURCE_DIR}/..
    ${GlViewer_SOURCE_DIR}/../Core/Include
    ${GlViewer_SOURCE_DIR}/../Model/Include
    ${GlViewer_SOURCE_DIR}/../GfxModel/Include
    ${GlViewer_SOURCE_DIR}/../../Algorithms/Include
    ${GlViewer_SOURCE_DIR}/../../3rdParty/glog/src
    ${GlViewer_SOURCE_DIR}/../../3rdParty/nano-signal-slot
)

set(SOURCE_FILES
    ${GlViewer_SOURCE_DIR}/Include/AppSettings.h
    ${GlViewer_SOURCE_DIR}/Include/BoxDialog.h
    ${GlViewer_SOURCE_DIR}/Include/GlProject.h
    ${GlViewer_SOURCE_DIR}/Include/GlViewerException.h
    ${GlViewer_SOURCE_DIR}/Include/GlViewObject.h
    ${GlViewer_SOURCE_DIR}/Include/GlWidget.h
    ${GlViewer_SOURCE_DIR}/Include/MainWindow.h
    ${GlViewer_SOURCE_DIR}/Include/Shader.h
    ${GlViewer_SOURCE_DIR}/Include/ShaderProgram.h
    ${GlViewer_SOURCE_DIR}/Include/StartUpPage.h

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
    ${GlViewer_SOURCE_DIR}/Source/StartUpPage.cpp
)

qt5_wrap_ui(UI_FILES
    ${GlViewer_SOURCE_DIR}/Form/BoxDialog.ui
    ${GlViewer_SOURCE_DIR}/Form/StartUpPage.ui
)

link_directories(
    ${GlViewer_SOURCE_DIR}/../../Algorithms
    ${GlViewer_SOURCE_DIR}/../../3rdParty/boost_1_57_0/lib
    ${GlViewer_SOURCE_DIR}/../../3rdParty/glog
)

add_executable(GlViewer ${SOURCE_FILES} ${UI_FILES})
target_link_libraries(GlViewer GfxModel Model Core Algo glog Qt5::Widgets Qt5::Gui Qt5::Core GLU stdc++)
