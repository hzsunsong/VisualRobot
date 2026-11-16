# @file GenerateConfigPackage.cmake
# @brief Exports a compiled library, generated export header, public headers, and package version metadata.

include_guard(GLOBAL)

macro(generate_config_package)
    # This is the default install directory for config-file package cmake files
    # It is relative to install prefix.
    set(${PROJECT_NAME}_INSTALL_CMAKEDIR cmake
    CACHE PATH "Installation directory for config-file package cmake files")

    set(CMAKE_DEBUG_POSTFIX "d")
    set(EXPORT_HEADER_FILENAME "${PROJECT_NAME}_export.h")
    string(TOLOWER ${EXPORT_HEADER_FILENAME} EXPORT_HEADER_FILENAME)

    include(GNUInstallDirs) # Defines the ${CMAKE_INSTALL_INCLUDEDIR} variable.
    # Include the GenerateExportHeader module for generating export headers.
    include(GenerateExportHeader)
    generate_export_header(${PROJECT_NAME} EXPORT_FILE_NAME ${EXPORT_HEADER_FILENAME})

    # Specify the include directories for the target named `${PROJECT_NAME}`
    target_include_directories(${PROJECT_NAME}
        PUBLIC
            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
            $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
    )

    # @brief Install target metadata and headers separately for downstream consumers.

    # @brief Associates the current library target with the package export set.
    # This will only install the output artifacts produced by the target.
    install(TARGETS ${PROJECT_NAME}
            EXPORT ${PROJECT_NAME}_export # Associate installed target files of the targets in TARGETS parameter with export name of ${PROJECT_NAME}_export
                                    # Note that this does not export anything yet, it just defines a name and context for the exports. The install(EXPORT)
                                    # call does the actual target exporting.
            # Will be appended to INTERFACE_INCLUDE_DIRECTORIES of exported targets.
            INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    )

    # Install the header files. Since header files are not listed as output artifacts, they have
    # to be installed separately.
    install (
        DIRECTORY ${PROJECT_SOURCE_DIR}/include/
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    )

    install(
        FILES ${CMAKE_CURRENT_BINARY_DIR}/${EXPORT_HEADER_FILENAME}
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}
    )

    # Generate the required import code for the content in <export name>
    # into ${PROJECT_NAME}-config.cmake CMake file. Prefix all target names with
    # ${PROJECT_NAME}:: while doing so (NAMESPACE). DESTINATION argument recommended to be a path
    # that will be picked by find_package() by default ('cmake' in our example).
    install(EXPORT ${PROJECT_NAME}_export
        FILE ${PROJECT_NAME}-config.cmake
        NAMESPACE ${PROJECT_NAME}::
        DESTINATION ${${PROJECT_NAME}_INSTALL_CMAKEDIR}
    )

    # Defines write_basic_package_version_file
    include(CMakePackageConfigHelpers)

    # Create a package version file for the package. This will allow consumers to specify
    # version argument to select between several versions of the package via specifying VERSION argument
    # to find_package().
    # We're not explicitly specifying the VERSION parameter to allow `write_basic_package_version_file` to
    # get it from project(...) version
    write_basic_package_version_file(
        "${PROJECT_NAME}-config-version.cmake"
        # @brief Accepts requested versions with the same major number that are not newer than this package.
        COMPATIBILITY SameMajorVersion
    )

    # Install command for deploying Config-file package files into the target system. It must be present
    # in the same directory as `${PROJECT_NAME}-config.cmake` file.
    install(
        FILES "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-config-version.cmake"
        DESTINATION "${${PROJECT_NAME}_INSTALL_CMAKEDIR}"
    )
endmacro()