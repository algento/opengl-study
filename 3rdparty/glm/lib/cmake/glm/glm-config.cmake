# Avoid repeatedly including the targets
if(NOT TARGET glm)
    # Provide path for scripts
    list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}")
    include(${CMAKE_CURRENT_LIST_DIR}/glm-targets.cmake)
    set(GLM_LIBRARIES glm)
    get_target_property(GLM_INCLUDE_DIRS glm INTERFACE_INCLUDE_DIRECTORIES)
endif()
