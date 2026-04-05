cmake_minimum_required(VERSION 3.15)
project(Pong LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include(FetchContent)

set(BUILD_SHARED_LIBS OFF CACHE BOOL "Build shared libraries" FORCE)
set(BUILD_EXAMPLES OFF CACHE BOOL "Build raylib examples" FORCE)
set(BUILD_GAMES OFF CACHE BOOL "Build raylib games" FORCE)
set(RAYLIB_INSTALL OFF CACHE BOOL "Disable raylib install target" FORCE)

FetchContent_Declare(
  raylib
  GIT_REPOSITORY https://github.com/raysan5/raylib.git
  GIT_TAG        5.5
)
FetchContent_MakeAvailable(raylib)

add_executable(Pong main.cpp button.cpp)
target_include_directories(Pong PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(Pong PRIVATE raylib)

if(APPLE)
  find_library(COCOA Cocoa REQUIRED)
  find_library(OPENGL OpenGL REQUIRED)
  find_library(IOKIT IOKit REQUIRED)
  find_library(COREVIDEO CoreVideo REQUIRED)
  target_link_libraries(Pong PRIVATE ${COCOA} ${OPENGL} ${IOKIT} ${COREVIDEO})
endif()
