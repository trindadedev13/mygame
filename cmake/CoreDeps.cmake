cmake_minimum_required(VERSION 3.22)

if(NOT TARGET SDL3::SDL3 OR NOT TARGET SDL3_image::SDL3_image OR NOT TARGET SDL3_ttf::SDL3_ttf)
  add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/../extern"
                   "${CMAKE_BINARY_DIR}/extern_build")
endif()

set(CORE_DEPS
    SDL3::SDL3
    SDL3_image::SDL3_image
    SDL3_ttf::SDL3_ttf)