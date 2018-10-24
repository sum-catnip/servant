cmake_minimum_required(VERSION 3.13) # cuz of the target_include_library changes
project(servant VERSION 0.1.0)

include(CTest)
enable_testing()

set(CMAKE_CXX_STANDARD 17)

if(MSVC)
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MDd")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MD")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /permissive")
    add_definitions(-D_CRT_SECURE_NO_WARNINGS) # we get it microsoft..
    add_definitions(-DHAVE_SNPRINTF) # python get your shit together
endif()

find_package(cpprestsdk CONFIG REQUIRED)

add_executable(servant)

add_subdirectory(inc)
add_subdirectory(src)

target_link_libraries(servant PRIVATE cpprestsdk::cpprest cpprestsdk::cpprestsdk_zlib_internal cpprestsdk::cpprestsdk_boost_internal cpprestsdk::cpprestsdk_openssl_internal)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)