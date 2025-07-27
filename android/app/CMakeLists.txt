cmake_minimum_required(VERSION 3.22)

project(mygame_android)

include(${CMAKE_CURRENT_SOURCE_DIR}/../../cmake/CoreDeps.cmake)

add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/../../core"
    "${CMAKE_BINARY_DIR}/core_build")

add_subdirectory(src/main/c)