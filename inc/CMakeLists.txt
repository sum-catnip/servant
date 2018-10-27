add_subdirectory(pybind11)

target_link_libraries(servant PRIVATE pybind11::embed)