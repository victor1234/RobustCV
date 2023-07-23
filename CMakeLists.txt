cmake_minimum_required(VERSION 3.16)

project(RobustCV VERSION 0.0.1)

# Create target
set(RCV_TARGET_NAME ${PROJECT_NAME})
add_library(${RCV_TARGET_NAME} INTERFACE)

# Add alias
add_library(${RCV_TARGET_NAME}::${RCV_TARGET_NAME} ALIAS ${RCV_TARGET_NAME})

# Include paths
target_include_directories(
    ${RCV_TARGET_NAME}
    INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

# Set compiler features
target_compile_features(${RCV_TARGET_NAME} INTERFACE cxx_std_17)
set_target_properties(${RCV_TARGET_NAME} PROPERTIES CXX_STANDARD_REQUIRED ON)

# Install rules
include(GNUInstallDirs)

# Create package targets file
install(TARGETS ${RCV_TARGET_NAME} EXPORT ${RCV_TARGET_NAME}-target)
install(
    EXPORT ${RCV_TARGET_NAME}-target
    FILE ${RCV_TARGET_NAME}-targets.cmake
    NAMESPACE ${RCV_TARGET_NAME}::
    DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/${PROJECT_NAME}/cmake
)

# Add tests
option(RCV_BUILD_TESTS "Build tests" OFF)
if(RCV_BUILD_TESTS)
    find_package(Catch2 3 REQUIRED)
    enable_testing()
    add_subdirectory(tests)
endif()

include(CMakePackageConfigHelpers)

# Create package config file
configure_package_config_file(
    "${PROJECT_SOURCE_DIR}/cmake/${RCV_TARGET_NAME}-config.cmake.in"
    "${RCV_TARGET_NAME}-config.cmake"
    INSTALL_DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/${PROJECT_NAME}/cmake
)

# Create package version file
write_basic_package_version_file(
    ${RCV_TARGET_NAME}-config-version.cmake
    COMPATIBILITY ExactVersion
)

# Install cmake files
install(
    FILES
        "${PROJECT_BINARY_DIR}/${RCV_TARGET_NAME}-config.cmake"
        "${PROJECT_BINARY_DIR}/${RCV_TARGET_NAME}-config-version.cmake"
    DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/${PROJECT_NAME}/cmake
)

# Install headers
install(DIRECTORY ${PROJECT_SOURCE_DIR}/RobustCV TYPE INCLUDE)
