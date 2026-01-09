cmake_minimum_required(VERSION 3.14)
project(MyVectorTest LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

include_directories(source)

Include(FetchContent)

FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2
  GIT_TAG        v3.11.0 # or a later release
)

FetchContent_MakeAvailable(Catch2)

add_executable(tests tests/tests_vector.cpp)

target_link_libraries(tests PRIVATE Catch2::Catch2WithMain)

include(CTest)
include(Catch)
catch_discover_tests(tests)


