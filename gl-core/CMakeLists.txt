CMAKE_MINIMUM_REQUIRED(VERSION 3.10)
project(gl-core)

## Settings ------------------------------------------------------------------##
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED true)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(3RDPARTY_DIR ${CMAKE_SOURCE_DIR}/3rdparty)

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED PATHS ${3RDPARTY_DIR}/glfw/lib/cmake/glfw3)
find_package(spdlog REQUIRED PATHS ${3RDPARTY_DIR}/spdlog/lib/cmake/spdlog)
find_package(imgui REQUIRED PATHS ${3RDPARTY_DIR}/imgui/lib/cmake/imgui)
find_package(glad REQUIRED PATHS ${3RDPARTY_DIR}/glad/lib/cmake/glad)
find_package(glm REQUIRED PATHS ${3RDPARTY_DIR}/glm/lib/cmake/glm)
find_package(fmt REQUIRED)
set(STB_IMAGE_INCUDE_DIRS ${3RDPARTY_DIR}/stb_image)

file(GLOB_RECURSE GLCORE_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/gl-core/*.cpp")

add_library(gl-core STATIC
    ${GLCORE_SOURCES}
)
set_target_properties(gl-core PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lib
)

target_include_directories(gl-core PUBLIC
    ${GLAD_INCLUDE_DIRS} 
    ${GLM_INCLUDE_DIRS}
    ${GLFW3_INCLUDE_DIRS}
    ${IMGUI_INCLUDE_DIRS}
    ${GLCORE_SRC_DIR}
)

target_link_libraries(gl-core PUBLIC 
    ${GLAD_LIBRARIES} 
    ${GLM_LIBRARIES}
    ${GLFW3_LIBRARIES}
    ${IMGUI_LIBRARIES}
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

target_compile_definitions(gl-core PUBLIC
    ${GLCORE_COMPILE_DEFINITIONS}
)