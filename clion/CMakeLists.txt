cmake_minimum_required(VERSION 3.13)
project(mucom88)

set(CMAKE_CXX_STANDARD 17)

file(GLOB SOURCES
        ../src/*.c
        ../src/*.cpp
        ../src/dummy/*.cpp
        ../src/fmgen/*.cpp
        ../src/module/*.cpp
        ../src/plugin/*.cpp
        ../src/sdl/*.cpp
        ../src/Z80/*.cpp)

list(FILTER SOURCES EXCLUDE REGEX "/src/mucomerror.cpp")
list(FILTER SOURCES EXCLUDE REGEX "/src/main.cpp")

find_path(SDL2_INCLUDE_DIR NAMES SDL.h PATH_SUFFIXES SDL2)

add_executable(${CMAKE_PROJECT_NAME} ${SOURCES})

target_include_directories(${CMAKE_PROJECT_NAME}
        PRIVATE ${SDL2_INCLUDE_DIR}
        PRIVATE ../src
        PRIVATE ../src/module)

target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE SDL2)
