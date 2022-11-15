include_guard()

function (${PROJECT_NAME}_add_test name)
  cmake_parse_arguments(ARG "" "" "TEST_FILES" ${ARGN})

  string(REPLACE "/" "_" name_underscore ${name})
  set(test_target_name ${PROJECT_NAME}_${name_underscore}_test)
  add_executable(${test_target_name} ${ARG_TEST_FILES})
  target_link_libraries(
    ${test_target_name} PRIVATE ${PROJECT_NAME}_${name_underscore} gtest_main
                                gmock)
  # TODO: don't always link with gmock and gtest_main
  gtest_discover_tests(${test_target_name})
endfunction ()
