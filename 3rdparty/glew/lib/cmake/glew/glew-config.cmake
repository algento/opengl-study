# Avoid repeatedly including the targets
if(NOT TARGET glew)
    # Provide path for scripts
    list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}")
    include(${CMAKE_CURRENT_LIST_DIR}/glew-targets.cmake)
    set(GLEW_LIBRARIES GLEW::glew)
    get_target_property(GLEW_INCLUDE_DIRS GLEW::glew INTERFACE_INCLUDE_DIRECTORIES)
endif()
