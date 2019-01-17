add_subdirectory("base")
add_subdirectory("python")

target_sources(servant PRIVATE
    "${CMAKE_CURRENT_LIST_DIR}/module_handler.h"
    "${CMAKE_CURRENT_LIST_DIR}/module_handler.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/logger.h"
    "${CMAKE_CURRENT_LIST_DIR}/logger.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/servant.h"
    "${CMAKE_CURRENT_LIST_DIR}/servant.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/main.cpp"
)

target_include_directories(servant PUBLIC "${CMAKE_CURRENT_LIST_DIR}")