cmake_minimum_required(VERSION 3.12)

project(Forge VERSION 1.0)

file(GLOB_RECURSE SOURCES "src/*.cpp")
file(GLOB_RECURSE HEADERS "src/*.hpp")

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(Forge ${SOURCES})
set_target_properties(Forge PROPERTIES LINKER_LANGUAGE CXX)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -D_DEBUG -g -Wall -Wpedantic")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall -Wpedantic")

target_include_directories(Forge PUBLIC "src/Core")
target_include_directories(Forge PUBLIC "src/Essentials")

target_link_libraries(Forge PUBLIC glfw)
target_link_libraries(Forge PUBLIC vulkan)