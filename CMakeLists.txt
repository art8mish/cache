cmake_minimum_required(VERSION 3.28.3)

set(TARGET cache)
project(${TARGET})

set(SRC_DIR src)
set(INCLUDE_DIR include)
set(TESTS_DIR tests)

set(LFU_TARGET lfu_cache)
set(LFU_TESTS_TARGET lfuc_tests)

set(PC_TARGET pcache)
set(PC_TESTS_TARGET pc_tests)

set(SRC_LFU ${SRC_DIR}/lfu_cache.cpp ${SRC_DIR}/api.cpp)
set(SRC_PC ${SRC_DIR}/pcache.cpp ${SRC_DIR}/api.cpp)
set(SRC_LFU_TESTS ${TESTS_DIR}/lfuc_tests.cpp ${SRC_DIR}/api.cpp)
set(SRC_PC_TESTS ${TESTS_DIR}/pc_tests.cpp ${SRC_DIR}/api.cpp)


# LFU
add_executable(${LFU_TARGET} ${SRC_LFU})
target_include_directories(${LFU_TARGET} PRIVATE ${INCLUDE_DIR})
target_compile_options(${LFU_TARGET} PRIVATE
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

# LFU tests
add_executable(${LFU_TESTS_TARGET} ${SRC_LFU_TESTS})
target_include_directories(${LFU_TESTS_TARGET} PRIVATE ${INCLUDE_DIR})
target_link_libraries(${LFU_TESTS_TARGET} PRIVATE gtest pthread)
target_compile_options(${LFU_TESTS_TARGET} PRIVATE
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

# PC
add_executable(${PC_TARGET} ${SRC_PC})
target_include_directories(${PC_TARGET} PRIVATE ${INCLUDE_DIR})
target_compile_options(${PC_TARGET} PRIVATE
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

# PC tests
add_executable(${PC_TESTS_TARGET} ${SRC_PC_TESTS})
target_include_directories(${PC_TESTS_TARGET} PRIVATE ${INCLUDE_DIR})
target_link_libraries(${PC_TESTS_TARGET} PRIVATE gtest pthread)
target_compile_options(${PC_TESTS_TARGET} PRIVATE
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
