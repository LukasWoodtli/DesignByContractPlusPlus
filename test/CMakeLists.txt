################################
# Unit Tests
################################
# Add test cpp file
add_executable( runUnitTests invariant_test.cpp precond_test.cpp postcond_test.cpp)
# Link test executable against gtest & gtest_main
target_link_libraries(runUnitTests gtest gtest_main)
add_test( runUnitTests runUnitTests )

# Trace Test
add_executable(traceTest trace_test.cpp)
add_test( run_trace_test ${CMAKE_CURRENT_SOURCE_DIR}/run_trace_test.py )
