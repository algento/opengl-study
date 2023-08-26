# Avoid repeatedly including the targets
if(NOT TARGET spdlog)
    # Provide path for scripts
    list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}")
    include(${CMAKE_CURRENT_LIST_DIR}/spdlog-targets.cmake)
    set(SPDLOG_LIBRARIES spdlog::spdlog)
    get_target_property(SPDLOG_INCLUDE_DIRS spdlog INTERFACE_INCLUDE_DIRECTORIES)
endif()
