project(GfxModel CXX)

set(SOURCE_FILES
    ${GfxModel_SOURCE_DIR}/Include/Extent.h
    ${GfxModel_SOURCE_DIR}/Include/GfxBox.h
    ${GfxModel_SOURCE_DIR}/Include/GfxProject.h
    ${GfxModel_SOURCE_DIR}/Include/GfxStl.h
    ${GfxModel_SOURCE_DIR}/Include/GraphicsObject.h
    ${GfxModel_SOURCE_DIR}/Include/ViewpointCamera.h

    ${GfxModel_SOURCE_DIR}/Source/Extent.cpp
    ${GfxModel_SOURCE_DIR}/Source/GfxBox.cpp
    ${GfxModel_SOURCE_DIR}/Source/GfxProject.cpp
    ${GfxModel_SOURCE_DIR}/Source/GfxStl.cpp
    ${GfxModel_SOURCE_DIR}/Source/GraphicsObject.cpp
    ${GfxModel_SOURCE_DIR}/Source/ViewpointCamera.cpp
)

include_directories(
    ${GfxModel_SOURCE_DIR}/Include
    ${Graphics_SOURCE_DIR}/Core/Include
    ${Graphics_SOURCE_DIR}/Model/Include
    ${Graphics_SOURCE_DIR}/../Algorithms/Include
    ${Graphics_SOURCE_DIR}/../3rdParty/glog/src
    ${Graphics_SOURCE_DIR}/../3rdParty/nano-signal-slot
)

add_library(GfxModel ${SOURCE_FILES})
