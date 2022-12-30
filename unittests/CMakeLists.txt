include(FetchContent)

FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG main)
set(gtest_force_shared_crt
    ON
    CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)
include(GoogleTest)
set_property(DIRECTORY ${googletest_SOURCE_DIR} PROPERTY EXCLUDE_FROM_ALL YES)

add_library(ctl_test_util INTERFACE include/ctl_test_util/scoped_trace.hpp)
# TODO: change install dir
target_include_directories(ctl_test_util
                           INTERFACE ${PROJECT_SOURCE_DIR}/unittests/include)
target_link_libraries(ctl_test_util INTERFACE gtest)

include(add-ctl-test)

add_subdirectory(core) # Intentionally first
add_subdirectory(adt)
add_subdirectory(concept)
add_subdirectory(container)
add_subdirectory(meta)
add_subdirectory(object)
