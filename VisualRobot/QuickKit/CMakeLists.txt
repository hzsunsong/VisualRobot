cmake_minimum_required(VERSION ${MINIMUM_CMAKE_VERSION})

project(QuickKit VERSION ${PROJECT_VERSION} LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

file(MAKE_DIRECTORY "${CMAKE_BINARY_DIR}/${PLUGINS_DIR}")
set(QT_QML_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${PLUGINS_DIR}")

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Quick)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Quick)

qt_add_library(${PROJECT_NAME} SHARED)
qt_add_qml_module(${PROJECT_NAME}
    URI ${PROJECT_NAME}
    VERSION 1.0
    QML_FILES MyItemControls.qml
    SOURCES myitem.cpp myitem.h
)

set_target_properties(${PROJECT_NAME} PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_compile_definitions(${PROJECT_NAME}
    PRIVATE
        $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>
)

target_include_directories(${PROJECT_NAME}
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        Qt${QT_VERSION_MAJOR}::Core
        Qt${QT_VERSION_MAJOR}::Quick
)

# @brief Deploy the backing shared library next to the executable.
#
# The generated QML plugin imports this library, so it must remain discoverable
# from the application directory.
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "$<TARGET_FILE:${PROJECT_NAME}>"
        "${CMAKE_BINARY_DIR}/${APPLICATION_DIR}"
    VERBATIM
)