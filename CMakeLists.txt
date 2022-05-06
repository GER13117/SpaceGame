cmake_minimum_required(VERSION 3.22)
project(SpaceGame)

set(CMAKE_CXX_STANDARD 23)

add_subdirectory(libs/raylib)

add_executable(${PROJECT_NAME} src/main.cpp src/Game.cpp src/include/Game.h src/include/CelestialBody.cpp src/include/CelestialBody.h)


target_link_libraries(${PROJECT_NAME} raylib)