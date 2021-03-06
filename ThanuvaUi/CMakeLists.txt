project(ThanuvaUi CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_PREFIX_PATH $ENV{QTDIR})

set(CMAKE_INCLUDE_CURRENT_DIR ON) # to inclue moc and ui files
set(CMAKE_AUTOMOC ON)

find_package(Qt5Core)
find_package(Qt5Gui)
find_package(Qt5Widgets)

if(WIN32)
    add_definitions(-DGOOGLE_GLOG_DLL_DECL=)
endif()

add_definitions(-DGLOG_NO_ABBREVIATED_SEVERITIES)

set(SOURCE_FILES
    ${ThanuvaUi_SOURCE_DIR}/Include/AppSettings.h
    ${ThanuvaUi_SOURCE_DIR}/Include/BoxDialog.h
    ${ThanuvaUi_SOURCE_DIR}/Include/ConeDialog.h
    ${ThanuvaUi_SOURCE_DIR}/Include/CylinderDialog.h
    ${ThanuvaUi_SOURCE_DIR}/Include/GeometryDialog.h
    ${ThanuvaUi_SOURCE_DIR}/Include/GeometryListUiModel.h
    ${ThanuvaUi_SOURCE_DIR}/Include/MainWindow.h
    ${ThanuvaUi_SOURCE_DIR}/Include/MeshDialog.h
    ${ThanuvaUi_SOURCE_DIR}/Include/OpenGLWidget.h
    ${ThanuvaUi_SOURCE_DIR}/Include/SphereDialog.h
    ${ThanuvaUi_SOURCE_DIR}/Include/StartUpPage.h

    ${ThanuvaUi_SOURCE_DIR}/Source/AppSettings.cpp
    ${ThanuvaUi_SOURCE_DIR}/Source/BoxDialog.cpp
    ${ThanuvaUi_SOURCE_DIR}/Source/ConeDialog.cpp
    ${ThanuvaUi_SOURCE_DIR}/Source/CylinderDialog.cpp
    ${ThanuvaUi_SOURCE_DIR}/Source/GeometryDialog.cpp
    ${ThanuvaUi_SOURCE_DIR}/Source/GeometryListUiModel.cpp
    ${ThanuvaUi_SOURCE_DIR}/Source/main.cpp
    ${ThanuvaUi_SOURCE_DIR}/Source/MainWindow.cpp
    ${ThanuvaUi_SOURCE_DIR}/Source/MeshDialog.cpp
    ${ThanuvaUi_SOURCE_DIR}/Source/OpenGLWidget.cpp
    ${ThanuvaUi_SOURCE_DIR}/Source/SphereDialog.cpp
    ${ThanuvaUi_SOURCE_DIR}/Source/StartUpPage.cpp
)

qt5_wrap_ui(UI_FILES
    ${ThanuvaUi_SOURCE_DIR}/Form/BoxDialog.ui
    ${ThanuvaUi_SOURCE_DIR}/Form/ConeDialog.ui
    ${ThanuvaUi_SOURCE_DIR}/Form/CylinderDialog.ui
    ${ThanuvaUi_SOURCE_DIR}/Form/MeshDialog.ui
    ${ThanuvaUi_SOURCE_DIR}/Form/SphereDialog.ui
    ${ThanuvaUi_SOURCE_DIR}/Form/StartUpPage.ui
)

include_directories(
    ${ThanuvaUi_SOURCE_DIR}/Include
    ${Thanuva_SOURCE_DIR}/Core/Include
    ${Thanuva_SOURCE_DIR}/Model/Include
    ${Thanuva_SOURCE_DIR}/Geometry/Include
    ${Thanuva_SOURCE_DIR}/Graphics/Include
    ${Thanuva_SOURCE_DIR}/../Algorithms/Include
    /usr/local/boost/include
    ${Thanuva_SOURCE_DIR}/../3rdParty/fmt/include
    # ${Thanuva_SOURCE_DIR}/../3rdParty/glog/src/windows
    ${Thanuva_SOURCE_DIR}/../3rdParty/glog
    ${Thanuva_SOURCE_DIR}/../3rdParty/nano-signal-slot
)

add_executable(ThanuvaUi ${SOURCE_FILES} ${UI_FILES})
if(WIN32)
target_link_libraries(ThanuvaUi
    Graphics Geometry Model Core
    ${Thanuva_SOURCE_DIR}/../Algorithms/MSVC/x64/Debug/Algo.lib
    ${Thanuva_SOURCE_DIR}/../3rdParty/fmt/build/fmt/Debug/fmt.lib
    ${Thanuva_SOURCE_DIR}/../3rdParty/glog/x64/Debug/libglog_static.lib
    Qt5::Widgets
    Qt5::Gui
    Qt5::Core
    glu32.lib
)
else()
target_link_libraries(ThanuvaUi
    Graphics Geometry Model Core
    ${Thanuva_SOURCE_DIR}/../Algorithms/libAlgo.a
    ${Thanuva_SOURCE_DIR}/../3rdParty/fmt/libfmt.a
    ${Thanuva_SOURCE_DIR}/../3rdParty/glog/libglog.a
    Qt5::Widgets
    Qt5::Gui
    Qt5::Core
    /usr/lib/x86_64-linux-gnu/libGLU.so.1
    /usr/lib/nvidia-384/libGL.so
    pthread
    stdc++fs
    stdc++
)
endif()
