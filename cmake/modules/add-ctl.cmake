include_guard()

function (${PROJECT_NAME}_add_component name)
  cmake_parse_arguments(ARG "" "INTERFACE_HEADER_FILES" "" ${ARGN})
  set(libname ${PROJECT_NAME}_${name})
  foreach (header ${ARG_INTERFACE_HEADER_FILES})
    add_library(${libname} INTERFACE
                ${PROJECT_SOURCE_DIR}/include/${PROJECT_NAME}/${name}/${header})
  endforeach ()
  add_library(${PROJECT_NAME}::${name} ALIAS ${libname})
  target_include_directories(
    ${libname} INTERFACE $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
                         $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)
  target_compile_features(${libname} INTERFACE cxx_std_20)

  install(
    TARGETS ${libname}
    EXPORT ${PROJECT_NAME}_targets
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

  string(TOUPPER ${PROJECT_NAME} PN_UP)
  set_property(GLOBAL APPEND PROPERTY ${PN_UP}_COMPONENTS ${name})

  install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/${PROJECT_NAME}/${name}
          DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME})
endfunction ()
