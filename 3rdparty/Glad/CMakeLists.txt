cmake_minimum_required (VERSION 3.8.2)

project(Glad LANGUAGES C)

add_library(${PROJECT_NAME} src/glad.c)

target_include_directories(${PROJECT_NAME} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")
