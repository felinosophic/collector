find_package(GTest REQUIRED)
find_package(OpenMP REQUIRED)

add_executable(run_tests
    setup_test.cpp  
    feed_reader_test.cpp
    test_embedder.cpp
)
target_include_directories(run_tests PUBLIC ${CMAKE_SOURCE_DIR}/include)

target_link_libraries(run_tests PRIVATE GTest::gtest_main collector_lib OpenMP::OpenMP_CXX)

include(GoogleTest)
gtest_discover_tests(run_tests
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)
