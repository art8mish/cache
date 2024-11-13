cmake_minimum_required(VERSION 3.28.3)

set(TARGET cache)
project(${TARGET})

set(LFU_TARGET lfu_cache)
set(PC_TARGET pcache)

set(SRC_DIR src)
set(INCLUDE_DIR include)

set(SRC_LFU ${SRC_DIR}/lfu_cache.cpp)
set(SRC_PC ${SRC_DIR}/pcache.cpp)


set(TESTS_DIR tests)

set(TESTS_LFU_TARGET lfu_tests)
set(TESTS_LFU_SRC ${TESTS_DIR}/lfuc_tests.cpp)

set(TESTS_PC_TARGET pc_tests)
set(TESTS_PC_SRC ${TESTS_DIR}/pc_tests.cpp)

#target_compile_features(${TARGET} cxx_std_20)

add_compile_options(
    #-D DEBUG
    #-g
    
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

include_directories(${INCLUDE_DIR})


add_executable(${LFU_TARGET} ${SRC_LFU})
add_executable(${PC_TARGET}  ${SRC_PC})

add_executable(${TESTS_LFU_TARGET} ${TESTS_LFU_SRC})
target_link_libraries(${TESTS_LFU_TARGET} gtest gmock pthread)

add_executable(${TESTS_PC_TARGET} ${TESTS_PC_SRC})
target_link_libraries(${TESTS_PC_TARGET} gtest gmock pthread)