cmake_minimum_required(VERSION 3.28.3)
set(TARGET cache_tests)

project(${TARGET})

set(SOURCE_EXE tests.cpp)

add_compile_options(
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

add_executable(${TARGET} ${SOURCE_EXE})
target_link_libraries(${TARGET} gtest gmock pthread) #for GTest