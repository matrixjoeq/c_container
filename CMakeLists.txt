# MIT License
#
# Copyright (c) 2017 MatrixJoeQ
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.

cmake_minimum_required(VERSION 3.0.2)
project(c_container)

include("common.cmake")
include_directories("include")

file(GLOB PRIME_SOURCES "prime/*.c")
c_static_library(c_prime "" ${PRIME_SOURCES})

file(GLOB CONTAINER_SOURCES "container/*.c")
c_static_library(c_container "" ${CONTAINER_SOURCES})

file(GLOB ALGORITHM_SOURCES "algorithm/*.c")
c_static_library(c_algorithm "" ${ALGORITHM_SOURCES})

file(GLOB UT_SOURCES "test/*.cpp")
cxx_gtest_executable(c_container_test "c_algorithm;c_container;c_prime" ${UT_SOURCES})
add_test(c_container c_container_test)

