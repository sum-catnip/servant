target_sources(servant
    PRIVATE
        "${CMAKE_CURRENT_LIST_DIR}/module.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/module.h"
        "${CMAKE_CURRENT_LIST_DIR}/capability.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/capability.h"
        "${CMAKE_CURRENT_LIST_DIR}/category.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/category.h"
        "${CMAKE_CURRENT_LIST_DIR}/parameter.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/parameter.h"
        "${CMAKE_CURRENT_LIST_DIR}/i_definable.h"
)

target_include_directories(servant PUBLIC "${CMAKE_CURRENT_LIST_DIR}")