add_library(meta INTERFACE
            ${PROJECT_SOURCE_DIR}/include/ctl/meta/type_traits.hpp)
add_library(${PROJECT_NAME}::meta ALIAS meta)
target_include_directories(
  meta INTERFACE $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
                 $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)
target_compile_features(meta INTERFACE cxx_std_20)

install(
  TARGETS meta
  EXPORT ${PROJECT_NAME}_targets
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

set_property(GLOBAL APPEND PROPERTY CTL_COMPONENTS meta)

install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/ctl/meta
        DESTINATION include_meta/ctl)
