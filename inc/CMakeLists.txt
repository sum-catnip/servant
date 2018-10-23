add_subdirectory(json)
add_subdirectory(pybind11)

target_link_libraries(servant PRIVATE pybind11::embed)
target_link_libraries(servant PRIVATE nlohmann_json::nlohmann_json)