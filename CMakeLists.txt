cmake_minimum_required(VERSION 3.5)
project(physics2d)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(FetchContent)
FetchContent_Declare(
    raylib
    GIT_REPOSITORY https://github.com/raysan5/raylib.git
    GIT_TAG 5.0
)
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(CMAKE_POLICY_VERSION_MINIMUM 3.5)
FetchContent_MakeAvailable(raylib)

add_executable(physics2d src/main.cpp)
target_link_libraries(physics2d PRIVATE raylib)

if(APPLE)
    target_link_libraries(physics2d PRIVATE "-framework IOKit" "-framework Cocoa" "-framework OpenGL")
endif()