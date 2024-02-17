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

# Optionally enable clang-tidy
find_program(CLANG_TIDY_EXE NAMES clang-tidy PATHS /opt/homebrew/opt/llvm/bin/)
if(NOT CLANG_TIDY_EXE)
  message(STATUS "clang-tidy not found. Skipping corresponding checks.")
else()
  set(CMAKE_CXX_CLANG_TIDY 
    ${CLANG_TIDY_EXE};
    -header-filter=.*;
    -checks=-*,portability-*,bugprone-*,-bugprone-easily-swappable-parameters,-bugprone-sizeof-expression,clang-analyzer-*;
  )
  message(STATUS "Found clang-tidy: ${CLANG_TIDY_EXE}.")
endif()

add_executable(roundtrip_test test/roundtrip_test.cpp)
target_include_directories(roundtrip_test PRIVATE include)

enable_testing()
add_test(NAME roundtrip_test COMMAND roundtrip_test)

# Add some more tests
include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        750d67d809700ae8fca6d610f7b41b71aa161808
  SYSTEM
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

set_target_properties(gtest PROPERTIES CXX_CLANG_TIDY "")
set_target_properties(gtest_main PROPERTIES CXX_CLANG_TIDY "")
set_target_properties(gmock PROPERTIES CXX_CLANG_TIDY "")
set_target_properties(gmock_main PROPERTIES CXX_CLANG_TIDY "")

add_executable(base64_tests test/base64_tests.cpp)
target_include_directories(base64_tests PRIVATE include)

target_link_libraries(base64_tests PRIVATE GTest::gtest GTest::gtest_main)

add_test(NAME base64_tests COMMAND base64_tests)


