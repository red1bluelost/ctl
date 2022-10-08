include_guard()

function (ctl_add_test name)
  cmake_parse_arguments(ARG "" "TEST_FILES" "" ${ARGN})

  add_executable(ctl_${name}_test ${ARG_TEST_FILES})
  target_link_libraries(ctl_${name}_test PRIVATE ctl_${name} gtest_main)
  gtest_discover_tests(ctl_${name}_test)
endfunction ()
