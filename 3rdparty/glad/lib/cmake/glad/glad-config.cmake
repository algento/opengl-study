# Avoid repeatedly including the targets
if(NOT TARGET glad)
    # Provide path for scripts
    list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}")
    include(${CMAKE_CURRENT_LIST_DIR}/glad-targets.cmake)
    set(GLAD_LIBRARIES glad)
    get_target_property(GLAD_INCLUDE_DIRS glad INTERFACE_INCLUDE_DIRECTORIES)
endif()
