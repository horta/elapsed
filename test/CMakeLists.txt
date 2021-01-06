add_executable(test_elapsed elapsed.c)
target_link_libraries(test_elapsed PRIVATE ELAPSED::elapsed)
target_compile_options(test_elapsed PRIVATE ${WARNING_FLAGS})
add_test(NAME elapsed COMMAND test_elapsed)

add_executable(test_elapsed_perf elapsed_perf.c)
target_link_libraries(test_elapsed_perf PRIVATE ELAPSED::elapsed)
target_compile_options(test_elapsed_perf PRIVATE ${WARNING_FLAGS})
add_test(NAME elapsed_perf COMMAND test_elapsed_perf)
