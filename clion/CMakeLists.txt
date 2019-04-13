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

if (EMSCRIPTEN)
    if (WEB_BROWSER)
        set(CMAKE_CXX_FLAGS "--pre-js ${PROJECT_SOURCE_DIR}/EmscriptenPre.js -s WASM=0")
        set(CMAKE_EXECUTABLE_SUFFIX ".html")
    else()
        set(CMAKE_CXX_FLAGS "-s NODERAWFS=1 -s WASM=0")
    endif()

    set(CMAKE_CXX_FLAGS_DEBUG "-g4")
    find_path(SDL_INCLUDE_DIR NAMES SDL.h PATH_SUFFIXES SDL)
else()
    find_path(SDL_INCLUDE_DIR NAMES SDL.h PATH_SUFFIXES SDL2)
endif()

add_executable(${CMAKE_PROJECT_NAME} ${SOURCES})

target_include_directories(${CMAKE_PROJECT_NAME}
        PRIVATE ${SDL_INCLUDE_DIR}
        PRIVATE ../src
        PRIVATE ../src/module)

if (NOT EMSCRIPTEN)
    target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE SDL2)
endif()