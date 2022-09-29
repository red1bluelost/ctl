cmake_minimum_required(VERSION 3.18)
project(
  ctl
  VERSION 0.0.0
  DESCRIPTION "The Clown Template Library for C++ utilities"
  HOMEPAGE_URL "http://github.com/red1bluelost/TODO")

# Global variable for all components
set_property(GLOBAL PROPERTY CTL_COMPONENTS)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(GNUInstallDirs)

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
  "CTLConfigVersion.cmake"
  VERSION ${PROJECT_VERSION}
  COMPATIBILITY SameMajorVersion)

configure_package_config_file(
  "${PROJECT_SOURCE_DIR}/cmake/${PROJECT_NAME}Config.cmake.in"
  "${PROJECT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
  INSTALL_DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/${PROJECT_NAME}/cmake)

add_subdirectory(lib)

get_property(CTL_COMPONENTS GLOBAL PROPERTY CTL_COMPONENTS)
message(STATUS "All components === ${CTL_COMPONENTS}")
