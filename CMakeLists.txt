cmake_minimum_required(VERSION 3.16)
project(PixeledSparrow)

#This sets the output directory for my built objects and makes sure the dinamic library goes into the build directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
add_subdirectory(external/SDL EXCLUDE_FROM_ALL)
add_subdirectory(external/SDL_image EXCLUDE_FROM_ALL)
add_executable(PixeledSparrow src/main.cpp src/helper.cpp src/animationProcess.cpp src/map.cpp)

target_link_libraries(PixeledSparrow PRIVATE SDL3_image::SDL3_image SDL3::SDL3)