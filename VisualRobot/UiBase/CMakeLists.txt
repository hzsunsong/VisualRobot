# @file CMakeLists.txt
# @brief Builds the shared QObject UI plugin base and QML component factory.

cmake_minimum_required(VERSION ${MINIMUM_CMAKE_VERSION})

project(UiBase VERSION ${PROJECT_VERSION} LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Qml)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Qml)

set(cxx_DIR "${SHARE_LIB_DIR}/cmake")

if(NOT TARGET cxx::cxx)
    find_package(cxx CONFIG REQUIRED)
endif()

add_library(${PROJECT_NAME} SHARED
  include/UiBase/applicationplugin.h src/UiBase/applicationplugin.cpp
)

add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

target_include_directories(${PROJECT_NAME}
  PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/include/${PROJECT_NAME}
    ${CMAKE_CURRENT_LIST_DIR}/src/${PROJECT_NAME}
    ${CMAKE_CURRENT_LIST_DIR}/../${SHARE_DIR}/include
    ${CMAKE_CURRENT_LIST_DIR}/../Ui/include
)

target_link_libraries(${PROJECT_NAME}
  PUBLIC
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Qml
    cxx::cxx
)

# @brief Deploy the built shared library to the application build directory.
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "$<TARGET_FILE:${PROJECT_NAME}>"
        "${CMAKE_CURRENT_BINARY_DIR}/../Application"
    VERBATIM
)

include(${CMAKE_CURRENT_LIST_DIR}/../cmake/GenerateConfigPackage.cmake)
generate_config_package()
