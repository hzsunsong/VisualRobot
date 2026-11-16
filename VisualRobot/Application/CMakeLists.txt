# @file CMakeLists.txt
# @brief Builds the Qt Quick executable and embeds the tabbed application shell.

cmake_minimum_required(VERSION ${MINIMUM_CMAKE_VERSION})

project(Application VERSION ${PROJECT_VERSION} LANGUAGES CXX)

# @brief Create plugin directories in the application build directory during configuration.
file(MAKE_DIRECTORY
    "${CMAKE_CURRENT_BINARY_DIR}/plugins/device"
    "${CMAKE_CURRENT_BINARY_DIR}/plugins/ui"
)

set(APP_NAME "VisualRobot")
set(spdlog_DIR "${THIRDPARTY_DIR}/spdlog/lib/cmake/spdlog")
set(cxx_DIR "${SHARE_LIB_DIR}/cmake")
set(Device_DIR "${SHARE_LIB_DIR}/cmake")
set(Ui_DIR "${SHARE_LIB_DIR}/cmake")

find_package(Qt6 REQUIRED COMPONENTS Quick)

if(NOT TARGET spdlog::spdlog)
    find_package(spdlog CONFIG REQUIRED)
endif()

if(NOT TARGET cxx::cxx)
    find_package(cxx CONFIG REQUIRED)
endif()

if(NOT TARGET Device::Device)
    find_package(Device CONFIG REQUIRED)
endif()

if(NOT TARGET Ui::Ui)
    find_package(Ui CONFIG REQUIRED)
endif()

qt_standard_project_setup(REQUIRES 6.10)

# Content of downloaded design from Figma to Qt or similar
if (EXISTS "${CMAKE_CURRENT_LIST_DIR}/importedcontent/CMakeLists.txt")
    add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/importedcontent)
endif()

qt_add_executable(${APP_NAME}
    main.cpp
)

qt_add_qml_module(${APP_NAME}
    URI Application
    QML_FILES
        Main.qml
        SOURCES application.h application.cpp
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(${APP_NAME} PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.${APP_NAME}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(${APP_NAME}
    PRIVATE
        Qt6::Quick
        cxx::cxx
        Device::Device
        Ui::Ui
)

install(TARGETS ${APP_NAME}
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
