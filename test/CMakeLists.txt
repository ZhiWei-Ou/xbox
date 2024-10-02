
enable_testing()

add_executable(
    xlog_test
    xlog_test.c
)
target_link_libraries(xlog_test PRIVATE xbox::xbox)

add_executable(
    xtime_test
    xtime_test.c
)
target_link_libraries(xtime_test PRIVATE xbox::xbox)
