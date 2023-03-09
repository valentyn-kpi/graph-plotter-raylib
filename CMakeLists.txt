cmake_minimum_required(VERSION 3.24)
project(Lab_3 C)

set(CMAKE_C_STANDARD 23)

set(CMAKE_CXX_FLAGS_RELEASE  "-Ofast")

include(FetchContent)
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE) # don't build the supplied examples
set(BUILD_GAMES    OFF CACHE BOOL "" FORCE) # don't build the supplied example games
FetchContent_Declare(raylib GIT_REPOSITORY https://github.com/raysan5/raylib.git GIT_TAG master)
FetchContent_MakeAvailable(raylib)

add_executable(Lab_3 main.c build_points/pnts.c build_points/pnts.h build_points/zoom.c build_points/zoom.h)

target_link_libraries(${PROJECT_NAME} PRIVATE raylib)

set(CMAKE_EXE_LINKER_FLAGS "-static")
