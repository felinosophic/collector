find_package(GTest REQUIRED)

add_executable(run_tests
    setup_test.cpp  
    feed_reader_test.cpp
)
target_include_directories(run_tests PUBLIC ${CMAKE_SOURCE_DIR}/include)

target_link_libraries(run_tests PRIVATE GTest::gtest_main collector_lib)

include(GoogleTest)
gtest_discover_tests(run_tests)
