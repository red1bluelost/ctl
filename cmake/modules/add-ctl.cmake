include_guard()

# TODO: add a bunch of comments to better document this
function (${PROJECT_NAME}_add_component name)
  string(TOUPPER ${PROJECT_NAME} PN_UP)

  cmake_parse_arguments(
    ARG "" "" "INTERFACE_HEADER_FILES;${PN_UP}_INTERFACE_DEPENDENCIES" ${ARGN})

  string(REPLACE "/" "_" name_underscore ${name})
  set(libname ${PROJECT_NAME}_${name_underscore})
  foreach (header ${ARG_INTERFACE_HEADER_FILES})
    list(APPEND INTERFACE_HEADERS_ABS_PATH
         ${PROJECT_SOURCE_DIR}/include/${PROJECT_NAME}/${name}/${header})
  endforeach ()
  add_library(${libname} INTERFACE ${INTERFACE_HEADERS_ABS_PATH})

  string(REPLACE "/" "::" name_colon ${name})
  add_library(${PROJECT_NAME}::${name_colon} ALIAS ${libname})

  target_include_directories(
    ${libname} INTERFACE $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
                         $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

  foreach (library ${ARG_${PN_UP}_INTERFACE_DEPENDENCIES})
    list(APPEND INTERFACE_DEPS ${PROJECT_NAME}_${library})
  endforeach ()
  target_link_libraries(
    ${libname}
    INTERFACE ${PROJECT_NAME}_config ${INTERFACE_DEPS}
    PUBLIC)

  target_compile_features(${libname} INTERFACE cxx_std_20)

  install(
    TARGETS ${libname}
    EXPORT ${PROJECT_NAME}_targets
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

  set_property(GLOBAL APPEND PROPERTY ${PN_UP}_COMPONENTS ${name_underscore})

  install(FILES ${INTERFACE_HEADERS_ABS_PATH}
          DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}/${name})
endfunction ()
