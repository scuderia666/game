cmake_minimum_required(VERSION 3.11)
project(game)

find_package(raylib 4.0.0 QUIET)

add_executable(${PROJECT_NAME} src/main.c)
target_link_libraries(${PROJECT_NAME} raylib)