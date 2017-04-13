cmake_minimum_required(VERSION 2.8.4)
project(c_container)

include("common.cmake")
include_directories("include" "container")

file(GLOB PRIME_SOURCES "prime/*.c")
c_static_library(c_prime "" ${PRIME_SOURCES})

file(GLOB CONTAINER_SOURCES "container/*.c")
c_static_library(c_container "" ${CONTAINER_SOURCES})

file(GLOB ALGORITHM_SOURCES "algorithm/*.c")
c_static_library(c_algorithm "" ${ALGORITHM_SOURCES})

file(GLOB UT_SOURCES "test/*.cpp")
set(gtest_flags "${gtest_flags} -DCONFIG_TRAVERSE")
cxx_gtest_executable(c_container_test "c_algorithm;c_container;c_prime" ${UT_SOURCES})
add_test(c_container c_container_test)

