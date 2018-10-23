target_sources(servant
    PRIVATE
    "${CMAKE_CURRENT_LIST_DIR}/python_bindings.h"
    "${CMAKE_CURRENT_LIST_DIR}/python_module.h"
    "${CMAKE_CURRENT_LIST_DIR}/python_module.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/python_capability.h"
    "${CMAKE_CURRENT_LIST_DIR}/python_capability.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/python_category.h"
    "${CMAKE_CURRENT_LIST_DIR}/python_category.cpp"
)

target_include_directories(servant PUBLIC "${CMAKE_CURRENT_LIST_DIR}")
#find_library(PYTHON_LIB python37 lib)