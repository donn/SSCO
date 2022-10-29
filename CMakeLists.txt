cmake_policy(SET CMP0110 NEW)
cmake_policy(SET CMP0048 NEW)
cmake_minimum_required(VERSION 3.10)

project(ssco VERSION 2.0.0 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(include)

add_library(ssco lib/ssco.cpp)

if (SSCO_TEST)
    add_executable(ssco_example example.cpp)
    target_link_libraries(ssco_example ssco)
endif()