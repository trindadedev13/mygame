cmake_minimum_required(VERSION 3.20)
project(mygame C)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=address -g")

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../core
    ${CMAKE_BINARY_DIR}/core_build)

add_executable(mygame
    src/main.c)

target_link_libraries(mygame
    PRIVATE
    mygame_core)