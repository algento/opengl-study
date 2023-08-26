# Avoid repeatedly including the targets
if(NOT TARGET imgui)
    # Provide path for scripts
    list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}")
    include(${CMAKE_CURRENT_LIST_DIR}/imgui-targets.cmake)
    set(IMGUI_LIBRARIES imgui)
    get_target_property(IMGUI_INCLUDE_DIRS imgui INTERFACE_INCLUDE_DIRECTORIES)
endif()
