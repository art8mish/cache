enable_testing()
find_package(GTest REQUIRED)
include(GoogleTest)

set(LFUC_SRC ${CMAKE_CURRENT_SOURCE_DIR}/lfuc_tests.cpp)
set(PC_SRC ${CMAKE_CURRENT_SOURCE_DIR}/pc_tests.cpp)

function(add_cache_test target)
    add_executable(${target} ${ARGN})
    target_include_directories(${target} PRIVATE ${INCLUDE_DIR})
    target_compile_features(${target} PRIVATE cxx_std_20)
    target_link_libraries(${target} PRIVATE GTest::gtest_main)
    gtest_discover_tests(${target})
endfunction()

add_cache_test(lfuc_tests ${LFUC_SRC})
add_cache_test(pc_tests ${PC_SRC})
