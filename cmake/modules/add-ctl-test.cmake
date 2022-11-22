include_guard()

function (${PROJECT_NAME}_add_test name)
  cmake_parse_arguments(
    ARG "" "" "${PN_UP}_TEST_DEPENDENCIES;LOCAL_HEADER_FILES;TEST_FILES"
    ${ARGN})

  string(REPLACE "/" "_" name_underscore ${name})
  set(test_target_name ${PROJECT_NAME}_${name_underscore}_test)
  add_executable(${test_target_name})

  target_sources(${test_target_name} PRIVATE ${ARG_TEST_FILES}
                                             ${ARG_LOCAL_HEADER_FILES})

  foreach (library ${ARG_${PN_UP}_TEST_DEPENDENCIES})
    list(APPEND TEST_DEPS ${PROJECT_NAME}_${library})
  endforeach ()

  target_link_libraries(
    ${test_target_name} PRIVATE ${PROJECT_NAME}_${name_underscore} gtest_main
                                gmock ${TEST_DEPS})
  # TODO: don't always link with gmock and gtest_main
  gtest_discover_tests(${test_target_name})
endfunction ()
