cmake_minimum_required(VERSION 3.16)

project(Visioneer LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenCV 4.5.3 REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

include_directories("src/")

add_subdirectory("3rdparty/spdlog")

option(GLFW_BUILD_EXAMPLES     OFF)
option(GLFW_BUILD_TESTS        OFF)
option(GLFW_BUILD_DOCS         OFF)
option(GLFW_INSTALL            OFF)
option(GLFW_DOCUMENT_INTERNALS OFF)
add_subdirectory("3rdparty/GLFW")

add_subdirectory("3rdparty/Glad")

# should demo window be compiled
set(IMGUI_DEMO ON)
add_subdirectory("3rdparty/imgui")

file(GLOB_RECURSE SOURCES "src/*")

add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME} glfw imgui Glad spdlog)
target_link_libraries(${PROJECT_NAME} opencv_imgcodecs opencv_dnn)
