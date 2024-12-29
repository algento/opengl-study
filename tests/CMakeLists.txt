cmake_minimum_required(VERSION 3.10)
project(gl-test)

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

add_executable(test_gui_event ${PROJECT_SOURCE_DIR}/gl-core/test_gui_event.cpp)

set_target_properties(test_gui_event PROPERTIES RUNTIME_OUTPUT_DIRECTORY
                                                ${CMAKE_SOURCE_DIR}/bin)

target_include_directories(test_gui_event PUBLIC ${GLCORE_SRC_DIR})

target_link_libraries(test_gui_event PUBLIC gl-core)
