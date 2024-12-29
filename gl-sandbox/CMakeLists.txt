cmake_minimum_required(VERSION 3.10)
project(gl-sandbox)

# CMake Settings-------------------------------------------------------------##
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED true)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(3RDPARTY_DIR ${CMAKE_SOURCE_DIR}/3rdparty)

# Dependencies --------------------------------------------------------------##
find_package(OpenGL REQUIRED)
# glfw3 -----------##
set(CMAKE_PREFIX_PATH ${3RDPARTY_DIR}/glfw/lib/cmake/glfw3) # To use library in
                                                            # 3rdparty
find_package(glfw3 CONFIG REQUIRED)
# find_package(glfw3 CONFIG REQUIRED PATHS ${3RDPARTY_DIR}/glfw/lib/cmake/glfw)
set(GLFW3_LIBRARIES glfw)
get_target_property(GLFW3_INCLUDE_DIRS glfw INTERFACE_INCLUDE_DIRECTORIES)
# glad -----------##
set(CMAKE_PREFIX_PATH ${3RDPARTY_DIR}/glad/lib/cmake/glad)
find_package(glad CONFIG REQUIRED)

file(GLOB_RECURSE GLSANDBOX_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

add_executable(gl-sandbox ${GLSANDBOX_SOURCES})

set_target_properties(gl-sandbox PROPERTIES RUNTIME_OUTPUT_DIRECTORY
                                            ${CMAKE_SOURCE_DIR}/bin)

target_include_directories(gl-sandbox PUBLIC ${GLCORE_SRC_DIR})

target_link_libraries(gl-sandbox PUBLIC gl-core)
