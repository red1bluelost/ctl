include_guard()

function (${PROJECT_NAME}_add_test name)
  cmake_parse_arguments(ARG "" "" "TEST_FILES" ${ARGN})

  add_executable(${PROJECT_NAME}_${name}_test ${ARG_TEST_FILES})
  target_link_libraries(${PROJECT_NAME}_${name}_test
                        PRIVATE ${PROJECT_NAME}_${name} gtest_main)
  gtest_discover_tests(${PROJECT_NAME}_${name}_test)
endfunction ()
