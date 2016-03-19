cmake_minimum_required(VERSION 3.0)

project(Thanuva CXX)

add_compile_options(-Wall -std=c++14)

if(WIN32)
    add_compile_options(-DWINDOWS)
elseif(APPLE)
    add_compile_options(-DAPPLE)
else()
    add_compile_options(-DLINUX)
endif()

add_subdirectory(Core)
add_subdirectory(Model)
add_subdirectory(Geometry)
add_subdirectory(Graphics)
add_subdirectory(ThanuvaUi)
