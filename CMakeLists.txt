cmake_minimum_required(VERSION 3.22)
project(SpaceGame)

set(CMAKE_CXX_STANDARD 23)

add_subdirectory(libs/raylib)

add_executable(${PROJECT_NAME} src/main.cpp
        src/Game.cpp src/include/Game.h
        src/CelestialBody.cpp src/include/CelestialBody.h
        src/Trajectories.cpp src/include/Trajectories.h src/include/cyber.h)


target_link_libraries(${PROJECT_NAME} raylib)

# Checks if OSX and links appropriate frameworks (only required on MacOS)
if (APPLE)
    target_link_libraries(${PROJECT_NAME} "-framework IOKit")
    target_link_libraries(${PROJECT_NAME} "-framework Cocoa")
    target_link_libraries(${PROJECT_NAME} "-framework OpenGL")
endif()