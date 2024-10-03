
enable_testing()

aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR} TEST_LIST)
foreach(test ${TEST_LIST})
    get_filename_component(test_name ${test} NAME_WE)

    add_executable(
        ${test_name}
        ${test}
    )
    target_link_libraries(${test_name} PRIVATE xbox::xbox)

endforeach()
