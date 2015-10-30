cmake_minimum_required(VERSION 2.8.11)

project(Graphics CXX)

add_compile_options(-Wall -std=c++14)

add_subdirectory(Core)
add_subdirectory(Model)
add_subdirectory(GfxModel)

find_package(Subversion)
if(SUBVERSION_FOUND)
    Subversion_WC_INFO(${Graphics_SOURCE_DIR} Graphics)
    add_definitions(-DREVISION_NUMBER=${Graphics_WC_REVISION})
endif()

add_subdirectory(GlViewer)
