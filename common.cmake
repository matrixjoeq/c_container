
#SET(CMAKE_BUILD_TYPE RELEASE)
SET(CMAKE_BUILD_TYPE DEBUG)

IF(NOT UNIX)
    message(FATAL_ERROR "OS not supported")
ENDIF()

enable_testing()

cmake_policy(SET CMP0045 OLD)

SET(CMAKE_C_COMPILER /usr/bin/gcc)
SET(CMAKE_CXX_COMPILER /usr/bin/g++)

set(common_base_flags "${common_base_flags} -Wall -Wextra")
set(common_base_flags "${common_base_flags} -Werror")

set(c_base_flags "${common_base_flags}")
set(c_base_flags "${c_base_flags} -std=gnu99")

set(cxx_base_flags "${common_base_flags}")
set(cxx_base_flags "${cxx_base_flags} -std=gnu++11")
set(cxx_base_flags "${cxx_base_flags} -Weffc++")

set(gtest_flags "${cxx_base_flags} -O0 -Wno-error -DGTEST_UNIT_TEST")
set(gmock_flags "${gtest_flags} -Wno-effc++")

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

# include path (all parent_directories)
function(include_parent_directories)
    get_directory_property(dir PARENT_DIRECTORY)
    while(dir)
        message(${dir})
        include_directories(${dir})
        get_directory_property(dir ${dir} PARENT_DIRECTORY)
    endwhile()
endfunction()

# basic library
function(add_library_ex name type flags libs)
    add_library(${name} ${type} ${ARGN})
    set_target_properties(${name}
        PROPERTIES
        COMPILE_FLAGS "${flags}"
        LINK_FLAGS "")
    target_link_libraries(${name} ${libs})
endfunction()

function(add_library_ex_with_whole_archive name type flags libs)
    add_library(${name} ${type} ${ARGN})
    set_target_properties(${name}
        PROPERTIES
        COMPILE_FLAGS "${flags}"
        LINK_FLAGS "-Wl,--whole-archive lib${name}.a -Wl,--no-whole-archive")
    target_link_libraries(${name} ${libs})
endfunction()

function(static_library name flags libs)
    add_library_ex(${name} STATIC "${flags}" "${libs}" ${ARGN})
endfunction()

function(static_library_with_whole_archive name flags libs)
    add_library_ex_with_whole_archive(${name} STATIC "${flags}" "${libs}" ${ARGN})
endfunction()

function(shared_library name flags libs)
    add_library_ex(${name} SHARED "${flags}" "${libs}" ${ARGN})
endfunction()

# c library
function(c_static_library name libs)
    static_library(${name} "${c_base_flags}" "${libs}" ${ARGN})
endfunction()

function(c_static_library_with_whole_archive name libs)
    static_library_with_whole_archive(${name} "${c_base_flags}" "${libs}" ${ARGN})
endfunction()

function(c_static_library_with_pic name libs)
    static_library(${name} "${c_base_flags} -fPIC" "${libs}" ${ARGN})
endfunction()

function(c_shared_library name libs)
    shared_library(${name} "${c_base_flags}" "${libs}" ${ARGN})
endfunction()

# c++ library
function(cxx_static_library name libs)
    static_library(${name} "${cxx_base_flags}" "${libs}" ${ARGN})
endfunction()

function(cxx_static_library_with_whole_archive name libs)
    static_library_with_whole_archive(${name} "${cxx_base_flags}" "${libs}" ${ARGN})
endfunction()

function(cxx_static_library_with_pic name libs)
    static_library(${name} "${cxx_base_flags} -fPIC" "${libs}" ${ARGN})
endfunction()

function(cxx_shared_library name libs)
    shared_library(${name} "${cxx_base_flags}" "${libs}" ${ARGN})
endfunction()

# executable
function(add_executable_ex name flags libs)
    add_executable(${name} ${ARGN})
    set_target_properties(${name}
        PROPERTIES
        COMPILE_FLAGS "${flags}")
    foreach(lib ${libs})
        get_target_property(prop ${lib} LINK_FLAGS)
        if(NOT prop)
            target_link_libraries(${name} ${lib})
        else(NOT prop)
            target_link_libraries(${name} ${prop} ${lib})
        endif(NOT prop)
    endforeach(lib)
endfunction()

function(c_executable name libs)
    add_executable_ex(${name} "${c_base_flags}" "${libs}" ${ARGN})
endfunction()

function(cxx_executable name libs)
    add_executable_ex(${name} "${cxx_base_flags}" "${libs}" ${ARGN})
endfunction()

# executable (gtest)
function(cxx_gtest_executable name libs)
    add_executable_ex(${name} "${gtest_flags}" "${libs};gtest_main;gtest;pthread" ${ARGN})
endfunction()

# executable (gmock)
function(cxx_gmock_executable name libs)
    add_executable_ex(${name} "${gmock_flags}" "${libs};gmock_main;gmock;gtest;pthread" ${ARGN})
endfunction()

