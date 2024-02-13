cmake_minimum_required(VERSION 3.25)
project(base64)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Warnings compiler flags
if(MSVC)
  add_compile_options(/W4)
else()
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

add_executable(roundtrip_test test/roundtrip_test.cpp)
target_include_directories(roundtrip_test PRIVATE include)

enable_testing()
add_test(NAME roundtrip_test COMMAND roundtrip_test)
