# @file CMakeLists.txt
# @brief Builds Qt UI plugin discovery and shared context support.

cmake_minimum_required(VERSION ${MINIMUM_CMAKE_VERSION})

project(Ui VERSION ${PROJECT_VERSION} LANGUAGES CXX)

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
  include/Ui/ui_def.h
  include/Ui/context.h
  include/Ui/iapplicationplugin.h
  include/Ui/uiset.h src/Ui/uiset.cpp
  include/Ui/uicontext.h src/Ui/uicontext.cpp
)

add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

target_include_directories(${PROJECT_NAME}
  PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/include/${PROJECT_NAME}
    ${CMAKE_CURRENT_LIST_DIR}/src/${PROJECT_NAME}
    ${CMAKE_CURRENT_LIST_DIR}/../${SHARE_DIR}/include
)

target_link_libraries(${PROJECT_NAME}
  PRIVATE
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Qml
    cxx::cxx
)

include(${CMAKE_CURRENT_LIST_DIR}/../cmake/GenerateConfigPackage.cmake)
generate_config_package()
