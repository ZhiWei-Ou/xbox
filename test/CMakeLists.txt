
enable_testing()

add_executable(
    xlog_test
    xlog_test.c
)
target_link_libraries(xlog_test PRIVATE xbox::xbox)
