include_guard()

function (ctl_add_component name)
  cmake_parse_arguments(ARG "" "INTERFACE_HEADER_FILES" "" ${ARGN})
  set(libname ${PROJECT_NAME}_${name})
  foreach (header ${ARG_INTERFACE_HEADER_FILES})
    add_library(${libname} INTERFACE
                ${PROJECT_SOURCE_DIR}/include/ctl/${name}/${header})
  endforeach ()
  add_library(${PROJECT_NAME}::${name} ALIAS ${libname})
  target_include_directories(
    ${libname}
    INTERFACE $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
              $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}_${name}>)
  target_compile_features(${libname} INTERFACE cxx_std_20)

  install(
    TARGETS ${libname}
    EXPORT ${PROJECT_NAME}_targets
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

  set_property(GLOBAL APPEND PROPERTY CTL_COMPONENTS ${name})

  install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/ctl/${name}
          DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}_${name}/ctl)
endfunction ()
