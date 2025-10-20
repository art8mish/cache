cmake_minimum_required(VERSION 3.12)
project(cache LANGUAGES CXX)
enable_testing()
 
set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(TESTS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/tests)

find_package(GTest REQUIRED)

function(add_cache_exec target)
    add_executable(${target} ${ARGN})
    target_include_directories(${target} PRIVATE ${INCLUDE_DIR})
    target_compile_features(${target} PRIVATE cxx_std_20)
    add_warnings(${target})
endfunction()

function(add_cache_test target)
    add_executable(${target} ${ARGN})
    target_include_directories(${target} PRIVATE ${INCLUDE_DIR})
    target_compile_features(${target} PRIVATE cxx_std_20)
    target_link_libraries(${target} PRIVATE GTest::gtest GTest::gtest_main)
    add_warnings(${target})
    add_test(NAME ${target} COMMAND ${target})
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

add_cache_test(lfuc_tests ${TESTS_DIR}/lfuc_tests.cpp)
add_cache_test(pc_tests ${TESTS_DIR}/pc_tests.cpp)