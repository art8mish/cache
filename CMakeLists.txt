cmake_minimum_required(VERSION 3.16)
project(cache LANGUAGES CXX)

set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(TESTS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/tests)

option(BUILD_TESTS "Build unit tests" ON)
if(BUILD_TESTS)
    add_subdirectory(${TESTS_DIR})
endif()

function(add_cache_exec target)
    add_executable(${target} ${ARGN})
    target_include_directories(${target} PRIVATE ${INCLUDE_DIR})
    target_compile_features(${target} PRIVATE cxx_std_20)
    add_warnings(${target})
endfunction()


function(add_warnings target)
    target_compile_options(${target} PRIVATE
        -Werror
        -Wall
        -Wextra
        -Wpedantic
        -Wcast-align
        -Wcast-qual
        -Wconversion
        -Wold-style-cast
        -Wredundant-decls
        -Wsign-conversion
        -Wsign-promo
    )
endfunction()

add_cache_exec(lfu_cache ${SRC_DIR}/lfu_cache.cpp)
add_cache_exec(pcache ${SRC_DIR}/pcache.cpp)



