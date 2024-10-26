cmake_minimum_required(VERSION 3.28.3)

set(TARGET lfu_cache)

project(${TARGET})

set(TESTS tests)
set(SOURCE_EXE main.cpp)

#target_compile_features(${TARGET} cxx_std_20)

add_compile_options(
    #-D DEBUG
    -std=c++20

    -Werror
    -Wall
    -Wextra
    -Wpedantic
    -Wcast-align
    -Wcast-qual
    -Wconversion
    -Wctor-dtor-privacy
    -Wenum-compare
    -Wfloat-equal
    -Wnon-virtual-dtor
    -Wold-style-cast
    -Woverloaded-virtual
    -Wredundant-decls
    -Wsign-conversion
    -Wsign-promo
)

include_directories(include)
add_subdirectory(${TESTS})

add_executable(${TARGET} ${SOURCE_EXE})