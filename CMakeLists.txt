cmake_minimum_required(VERSION 3.15.0)
project(Test VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(FILES_TO_TEST a.cpp)
set(UNIT_TESTS main_test.cpp fixture_test.cpp gmock_test.cpp)
set(MAIN_FILE main.cpp)

add_subdirectory(googletest/)
include_directories(googletest/include)
add_library(test_files ${FILES_TO_TEST})

enable_testing()
add_executable(main ${MAIN_FILE})
target_link_libraries(main test_files)

add_executable(unit_test ${UNIT_TESTS})
target_link_libraries(unit_test gtest gtest_main rt pthread test_files gmock gmock_main)

include(GoogleTest)
gtest_discover_tests(unit_test)
