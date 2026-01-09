cmake_minimum_required(VERSION 3.14)

project(MyVectorTest LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

include_directories(source)
include_directories(extras)

add_library(Catch2 STATIC extras/catch_amalgamated.cpp)
target_include_directories(Catch2 PUBLIC extras)

add_executable(tests_vector tests/tests_vector.cpp)
target_link_libraries(tests_vector PRIVATE Catch2)

add_executable(tests_vector_iterator tests/tests_vector_iterator.cpp)
target_link_libraries(tests_vector_iterator PRIVATE Catch2)

# Enable CTest
enable_testing()

add_test(NAME VectorTests COMMAND tests_vector)
add_test(NAME VectorIteratorTests COMMAND tests_vector_iterator)