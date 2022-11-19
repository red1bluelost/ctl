include(FindDoxygen)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/doxygen.cfg.in
               ${CMAKE_CURRENT_BINARY_DIR}/doxygen.cfg @ONLY)

add_custom_target(
  doxygen-ctl
  COMMAND ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/doxygen.cfg
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  COMMENT "Generating ctl doxygen documentation."
  VERBATIM)

# TODO: add install of documentation
