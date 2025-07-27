file(GLOB_RECURSE
    CORE_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c")

include(${CMAKE_CURRENT_SOURCE_DIR}/../cmake/CheckPackages.cmake)
include(${CMAKE_CURRENT_SOURCE_DIR}/../cmake/CoreDeps.cmake)

add_library(mygame_core
    STATIC
    ${CORE_SOURCES})

target_include_directories(mygame_core
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(mygame_core
    PUBLIC
    ${CORE_DEPS})