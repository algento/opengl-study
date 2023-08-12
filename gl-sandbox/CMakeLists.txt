CMAKE_MINIMUM_REQUIRED(VERSION 3.10)
project(gl-sandbox)

## Settings ------------------------------------------------------------------##
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED true)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(3RDPARTY_DIR ${CMAKE_SOURCE_DIR}/3rdparty)

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED PATHS ${3RDPARTY_DIR}/glfw/lib/cmake/glfw3)
# find_package(spdlog REQUIRED PAHTS ${3RDPARTY_DIR}/spdlog/lib/cmake/spdlog)
# find_package(imgui REQUIRED PAHTS ${3RDPARTY_DIR}/imgui/lib/cmake/imgui)
# find_package(glad REQUIRED PAHTS ${3RDPARTY_DIR}/glad/lib/cmake/glad)
# find_package(glm REQUIRED PAHTS ${3RDPARTY_DIR}/glm/lib/cmake/glm)
# find_package(fmt REQUIRED)
# set(STB_IMAGE_INCUDE_DIRS ${3RDPARTY_DIR}//stb_image)

file(GLOB_RECURSE GLSANDBOX_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

add_executable(gl-sandbox 
    ${GLSANDBOX_SOURCES}
)

set_target_properties(gl-sandbox PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin
)

target_include_directories(gl-sandbox PUBLIC
    ${GLCORE_SRC_DIR}
)

target_link_libraries(gl-sandbox PUBLIC 
    gl-core 
)