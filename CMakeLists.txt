# SPDX-License-Identifier: MIT
# SPDX-FileCopyrightText: Copyright (c) 2014-2024 Markus Mayer
#
# Part of kalman-clib — https://github.com/sunsided/kalman-clib
# Licensed under the MIT License. See LICENSE.md in the project root for details.

cmake_minimum_required(VERSION 3.25)
project(kalman_clib
        VERSION 1.0.0
        DESCRIPTION "Microcontroller-targeted naive Kalman filter in pure C"
        HOMEPAGE_URL "https://github.com/sunsided/kalman-clib"
        LANGUAGES C)

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

# ── Library ──────────────────────────────────────────────────────────────────

add_library(kalman_clib)
target_sources(kalman_clib PRIVATE
        src/cholesky.c
        src/kalman.c
        src/matrix.c)
target_include_directories(kalman_clib PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)
target_compile_features(kalman_clib PRIVATE c_std_11)
target_link_libraries(kalman_clib PUBLIC m)

set_target_properties(kalman_clib PROPERTIES
        SPDX_LICENSE_IDENTIFIER "MIT"
        SPDX_COPYRIGHT_TEXT     "Copyright (c) 2014-2024 Markus Mayer"
        PROJECT_URL             "https://github.com/sunsided/kalman-clib")

# ── Example (optional) ───────────────────────────────────────────────────────

option(KALMAN_CLIB_BUILD_EXAMPLES "Build example programs" ON)
if(KALMAN_CLIB_BUILD_EXAMPLES)
    add_executable(example
            src/kalman_example.c
            src/kalman_example_gravity.c
            src/main.c
            src/matrix_unittests.c)
    target_include_directories(example PRIVATE include src)
    target_link_libraries(example PRIVATE kalman_clib m)
    target_compile_features(example PRIVATE c_std_11)

    set_target_properties(example PROPERTIES
            SPDX_LICENSE_IDENTIFIER "MIT"
            SPDX_COPYRIGHT_TEXT     "Copyright (c) 2014-2024 Markus Mayer"
            PROJECT_URL             "https://github.com/sunsided/kalman-clib")
endif()

# ── Install ──────────────────────────────────────────────────────────────────

install(TARGETS kalman_clib
        EXPORT kalman_clibTargets
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

install(DIRECTORY include/
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
        FILES_MATCHING PATTERN "*.h")

install(FILES LICENSE.md
        DESTINATION ${CMAKE_INSTALL_DOCDIR})

# ── CMake package config ─────────────────────────────────────────────────────

install(EXPORT kalman_clibTargets
        FILE kalman_clibTargets.cmake
        NAMESPACE kalman_clib::
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/kalman_clib)

configure_package_config_file(
        cmake/kalman_clibConfig.cmake.in
        ${CMAKE_CURRENT_BINARY_DIR}/kalman_clibConfig.cmake
        INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/kalman_clib)

write_basic_package_version_file(
        ${CMAKE_CURRENT_BINARY_DIR}/kalman_clibConfigVersion.cmake
        VERSION ${PROJECT_VERSION}
        COMPATIBILITY SameMajorVersion)

install(FILES
        ${CMAKE_CURRENT_BINARY_DIR}/kalman_clibConfig.cmake
        ${CMAKE_CURRENT_BINARY_DIR}/kalman_clibConfigVersion.cmake
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/kalman_clib)
