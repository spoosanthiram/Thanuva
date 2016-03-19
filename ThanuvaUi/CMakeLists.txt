project(ThanuvaUi CXX)

set(CMAKE_PREFIX_PATH $ENV{QTDIR})

set(CMAKE_INCLUDE_CURRENT_DIR ON) # to inclue moc and ui files
set(CMAKE_AUTOMOC ON)

find_package(Qt5Core)
find_package(Qt5Gui)
find_package(Qt5Widgets)

set(SOURCE_FILES
    ${ThanuvaUi_SOURCE_DIR}/Include/AppSettings.h
    ${ThanuvaUi_SOURCE_DIR}/Include/BoxDialog.h
    ${ThanuvaUi_SOURCE_DIR}/Include/GlWidget.h
    ${ThanuvaUi_SOURCE_DIR}/Include/MainWindow.h
    ${ThanuvaUi_SOURCE_DIR}/Include/StartUpPage.h

    ${ThanuvaUi_SOURCE_DIR}/Source/AppSettings.cpp
    ${ThanuvaUi_SOURCE_DIR}/Source/BoxDialog.cpp
    ${ThanuvaUi_SOURCE_DIR}/Source/GlWidget.cpp
    ${ThanuvaUi_SOURCE_DIR}/Source/main.cpp
    ${ThanuvaUi_SOURCE_DIR}/Source/MainWindow.cpp
    ${ThanuvaUi_SOURCE_DIR}/Source/StartUpPage.cpp
)

qt5_wrap_ui(UI_FILES
    ${ThanuvaUi_SOURCE_DIR}/Form/BoxDialog.ui
    ${ThanuvaUi_SOURCE_DIR}/Form/StartUpPage.ui
)

include_directories(
    ${ThanuvaUi_SOURCE_DIR}/Include
    ${Thanuva_SOURCE_DIR}/Core/Include
    ${Thanuva_SOURCE_DIR}/Model/Include
    ${Thanuva_SOURCE_DIR}/Geometry/Include
    ${Thanuva_SOURCE_DIR}/Graphics/Include
    ${Thanuva_SOURCE_DIR}/../Algorithms/Include
    ${Thanuva_SOURCE_DIR}/../3rdParty/glog/src
    ${Thanuva_SOURCE_DIR}/../3rdParty/nano-signal-slot
)

add_executable(ThanuvaUi ${SOURCE_FILES} ${UI_FILES})
target_link_libraries(ThanuvaUi
    Graphics Geometry Model Core
    ${Thanuva_SOURCE_DIR}/../Algorithms/libAlgo.a
    ${Thanuva_SOURCE_DIR}/../3rdParty/glog/libglog.a
    Qt5::Widgets
    Qt5::Gui
    Qt5::Core
    GLU
    stdc++
)
