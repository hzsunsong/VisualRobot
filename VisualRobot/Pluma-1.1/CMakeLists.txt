cmake_minimum_required(VERSION 3.16)

set(CMAKE_INSTALL_PREFIX "${CMAKE_CURRENT_LIST_DIR}/../share"
  CACHE PATH "Installation directory." FORCE)

project(Pluma VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_DEBUG_POSTFIX "d")
set(BUILD_SHARED_LIBS OFF)

add_library(${PROJECT_NAME} STATIC
  src/Pluma/Dir.cpp
  src/Pluma/DLibrary.cpp
  src/Pluma/Host.cpp
  src/Pluma/PluginManager.cpp
  src/Pluma/Provider.cpp
)

target_include_directories(${PROJECT_NAME}
  PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/include
  PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/src
)

target_compile_definitions(${PROJECT_NAME}
  PRIVATE
    PLUMA_STATIC
)

include(GNUInstallDirs)
install(TARGETS ${PROJECT_NAME}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

install(
  DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/include
  DESTINATION ${CMAKE_INSTALL_PREFIX}
)
