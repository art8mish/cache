cmake_minimum_required(VERSION 3.28.3)

set(TARGET cache)
project(${TARGET})

set(SRC_DIR src)
set(INCLUDE_DIR include)
set(TESTS_DIR tests)

set(LFU_TARGET lfu_cache)
set(LFU_TESTS_TARGET lfu_tests)

set(PC_TARGET pcache)
set(PC_TESTS_TARGET pc_tests)

set(SRC_LFU ${SRC_DIR}/lfu_cache.cpp)
set(SRC_PC ${SRC_DIR}/pcache.cpp)
set(SRC_LFU_TESTS ${TESTS_DIR}/lfuc_tests.cpp)
set(SRC_PC_TESTS ${TESTS_DIR}/pc_tests.cpp)


target_compile_options(${LFU_TARGET}
    # -g
    -D NDEBUG
    -std=c++20

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

target_compile_options(${PC_TARGET}
    # -g
    -D NDEBUG
    -std=c++20

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

target_compile_options(${LFU_TESTS_TARGET}
    # -g
    -DNDEBUG
    -std=c++20

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

target_compile_options(${PC_TESTS_TARGET}
    # -g
    -DNDEBUG
    -std=c++20

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


target_include_directories(${LFU_TARGET} ${INCLUDE_DIR})
target_include_directories(${PC_TARGET} ${INCLUDE_DIR})
target_include_directories(${LFU_TESTS_TARGET} ${INCLUDE_DIR})
target_include_directories(${PC_TESTS_TARGET} ${INCLUDE_DIR})


add_executable(${LFU_TARGET} ${SRC_LFU})
add_executable(${PC_TARGET}  ${SRC_PC})

add_executable(${LFU_TESTS_TARGET} ${SRC_LFU_TESTS})
target_link_libraries(${LFU_TESTS_TARGET} gtest pthread)

add_executable(${PC_TESTS_TARGET} ${SRC_PC_TESTS})
target_link_libraries(${PC_TESTS_TARGET} gtest pthread)