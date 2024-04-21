CMAKE_MINIMUM_REQUIRED(VERSION 3.10)
project(gl-core)

## CMake Settings-------------------------------------------------------------##
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED true)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(3RDPARTY_DIR ${CMAKE_SOURCE_DIR}/3rdparty)
## To use library in 3rdparty --------##
# set(CMAKE_FIND_ROOT_PATH ${3RDPARTY_DIR}) # for find_package 
# set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE "ONLY") # for find_package, no search in system paths
# set(CMAKE_PREFIX_PATH ${3RDPARTY_DIR}/glfw/lib/cmake/glfw3) # To use library in 3rdparty
# find_package(glfw3 CONFIG REQUIRED)
# OR
# find_package(glfw3 CONFIG REQUIRED PATHS ${3RDPARTY_DIR}/glfw/lib/cmake/glfw3 NO_DEFAULT_PATH)
## ----------------------------------##

## Dependencies --------------------------------------------------------------##
find_package(OpenGL REQUIRED)
find_package(fmt REQUIRED)

## Custom-build 3rd parth libraries -----------##
set(STB_IMAGE_INCUDE_DIRS ${3RDPARTY_DIR})

find_package(glfw3 CONFIG REQUIRED  PATHS ${3RDPARTY_DIR}/glfw/lib/cmake/glfw3 NO_DEFAULT_PATH)
set(GLFW3_LIBRARIES glfw)
get_target_property(GLFW3_INCLUDE_DIRS glfw INTERFACE_INCLUDE_DIRECTORIES)

find_package(glad CONFIG REQUIRED PATHS ${3RDPARTY_DIR}/glad/lib/cmake/glad NO_DEFAULT_PATH)

find_package(glm CONFIG REQUIRED PATHS ${3RDPARTY_DIR}/glm/lib/cmake/glm NO_DEFAULT_PATH)

find_package(imgui CONFIG REQUIRED PATHS ${3RDPARTY_DIR}/imgui/lib/cmake/imgui NO_DEFAULT_PATH)

find_package(glew CONFIG REQUIRED PATHS ${3RDPARTY_DIR}/glew/lib/cmake/glew NO_DEFAULT_PATH)

find_package(spdlog CONFIG REQUIRED PATHS ${3RDPARTY_DIR}/spdlog/lib/cmake/spdlog NO_DEFAULT_PATH)

find_package(assimp CONFIG REQUIRED PATHS ${3RDPARTY_DIR}/assimp/lib/cmake/assimp-5.4 NO_DEFAULT_PATH)

## Target ------------------------------------------------------------------##
file(GLOB_RECURSE GLCORE_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/gl-core/*.cpp")

add_library(gl-core STATIC
${GLCORE_SOURCES}
)
set_target_properties(gl-core PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lib
)

set(GLCORE_INCLUDE_DIR_LISTS 
    ${GLCORE_SRC_DIR}
    ${GLAD_INCLUDE_DIRS} 
    ${GLFW3_INCLUDE_DIRS}
    ${STB_IMAGE_INCUDE_DIRS}
    ${ASSIMP_INCLUDE_DIRS}
    # ${IMGUI_INCLUDE_DIRS}
)
if(USE_GLEW)
    list(APPEND GLCORE_INCLUDE_DIR_LISTS 
        ${GLCORE_INCLUDE_DIR_LISTS} ${GLEW_INCLUDE_DIRS})
else()
    list(APPEND GLCORE_INCLUDE_DIR_LISTS 
        ${GLCORE_INCLUDE_DIR_LISTS} ${GLAD_INCLUDE_DIRS})
endif()
target_include_directories(gl-core PUBLIC
    ${GLCORE_INCLUDE_DIR_LISTS} 
)

set(GLCORE_LIBRARY_LISTS 
    ${GLM_LIBRARIES}
    ${GLFW3_LIBRARIES}
    ${ASSIMP_LIBRARIES}
    # ${IMGUI_LIBRARIES}
)
if(USE_GLEW)
    list(APPEND GLCORE_LIBRARY_LISTS ${GLCORE_LIBRARY_LISTS} ${GLEW_LIBRARIES})
else()
    list(APPEND GLCORE_LIBRARY_LISTS ${GLCORE_LIBRARY_LISTS} ${GLAD_LIBRARIES})
endif()
target_link_libraries(gl-core PUBLIC 
    ${GLCORE_LIBRARY_LISTS}
)

target_precompile_headers(gl-core PRIVATE
    ${GLCORE_SRC_DIR}/gl-core/glpch.h
)

set(GLCORE_COMPILE_DEFINITIONS 
    GLFW_INCLUDE_NONE
    SPDLOG_ENABLE_ASSERTS
    ENABLE_PROFILING
    STB_IMAGE_IMPLEMENTATION
)

if(WIN32)
    set(GLCORE_COMPILE_DEFINITIONS ${GLCORE_COMPILE_DEFINITIONS}
        PLATFORM_WINDOWS
    )
elseif(UNIX AND NOT APPLE)
    set(GLCORE_COMPILE_DEFINITIONS ${GLCORE_COMPILE_DEFINITIONS}
        PLATFORM_UNIX
    )
elseif(APPLE)
    set(GLCORE_COMPILE_DEFINITIONS ${GLCORE_COMPILE_DEFINITIONS}
        PLATFORM_APPLE
    )
endif()

if(USE_GLEW)
    set(FLAG_GLEW 1)
else()
    set(FLAG_GLEW 0)
endif()

target_compile_definitions(gl-core PUBLIC
    ${GLCORE_COMPILE_DEFINITIONS}
    GLCORE_USE_GLEW=${FLAG_GLEW}
)