cmake_minimum_required(VERSION 3.10)
project(base64)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

add_executable(roundtrip_test test/roundtrip_test.cpp)
target_compile_options(roundtrip_test PRIVATE -Wall -Wextra -Woverflow -Wconversion)
add_custom_command(TARGET roundtrip_test POST_BUILD COMMAND ./roundtrip_test WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
