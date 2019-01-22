set(JSON_BuildTests OFF CACHE INTERNAL "")

add_subdirectory(pybind11)
add_subdirectory(nlohmann_json)

target_link_libraries(servant PRIVATE pybind11::embed)
target_link_libraries(servant PRIVATE nlohmann_json::nlohmann_json)