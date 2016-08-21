cmake_minimum_required(VERSION 2.6)
project(DesignByContractPlusPlus)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -fno-elide-constructors -pedantic-errors -Wextra -Wall     -Winit-self -Wold-style-cast -Woverloaded-virtual -Wuninitialized -Wmissing-declarations    -Winit-self -std=c++11")


################################
# GTest
################################
# We need thread support
#find_package(Threads REQUIRED)

ADD_SUBDIRECTORY (googletest)
enable_testing()

# http://stackoverflow.com/a/736838/1272072
#add_custom_target(check ${CMAKE_CTEST_COMMAND} -V)
#add_custom_target(check ${MAKE_CTEST_COMMAND} DEPENDS runUnitTests)

include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})


################################
# Unit Tests
################################
# Add test cpp file
add_executable( runUnitTests test.cpp )
# Link test executable against gtest & gtest_main
target_link_libraries(runUnitTests gtest gtest_main)
add_test( runUnitTests runUnitTests )

