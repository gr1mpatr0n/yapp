cmake_minimum_required(VERSION 3.14)
project(yapp-cpp VERSION 1.0.0 LANGUAGES CXX)

add_library(yapp INTERFACE)
add_library(yapp::yapp ALIAS yapp)

target_include_directories(yapp INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

target_compile_features(yapp INTERFACE cxx_std_17)

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

install(FILES include/yapp.hpp
              include/parser.hpp
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(TARGETS yapp
    EXPORT yapp-targets
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(EXPORT yapp-targets
    FILE yapp-targets.cmake
    NAMESPACE yapp::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/yapp
)

configure_package_config_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake/yapp-config.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/yapp-config.cmake
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/yapp
)

write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/yapp-config-version.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/yapp-config.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/yapp-config-version.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/yapp
)

option(YAPP_BUILD_EXAMPLES "Build example programs" OFF)
if(YAPP_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()
