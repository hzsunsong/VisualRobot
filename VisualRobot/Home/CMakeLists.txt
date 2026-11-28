# @file CMakeLists.txt
# @brief Builds the robot-selection UI plugin and embeds its page resources.

cmake_minimum_required(VERSION ${MINIMUM_CMAKE_VERSION})

project(Home VERSION ${PROJECT_VERSION} LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core)

set(cxx_DIR "${SHARE_LIB_DIR}/cmake")
set(UiBase_DIR "${SHARE_LIB_DIR}/cmake")

if(NOT TARGET cxx::cxx)
    find_package(cxx CONFIG REQUIRED)
endif()

if(NOT TARGET UiBase::UiBase)
    find_package(UiBase CONFIG REQUIRED)
endif()

add_library(${PROJECT_NAME} SHARED
  res.qrc
  include/Home/homeplugin.h src/Home/homeplugin.cpp
)

target_include_directories(${PROJECT_NAME}
  PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/include/${PROJECT_NAME}
    ${CMAKE_CURRENT_LIST_DIR}/src/${PROJECT_NAME}
    ${CMAKE_CURRENT_LIST_DIR}/../${SHARE_DIR}/include
    ${CMAKE_CURRENT_LIST_DIR}/../Ui/include
)

target_link_libraries(${PROJECT_NAME}
  PRIVATE
    Qt${QT_VERSION_MAJOR}::Core
    cxx::cxx
    UiBase::UiBase
)

# @brief Deploy the built UI plugin to the application plugin directory.
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "$<TARGET_FILE:${PROJECT_NAME}>"
        "${CMAKE_CURRENT_BINARY_DIR}/../${PLUGINS_DIR}/ui"
    VERBATIM
)

include(GNUInstallDirs)

install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)