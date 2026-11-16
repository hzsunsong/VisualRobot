# @file CMakeLists.txt
# @brief Builds the device plugin host and exports its public package.

cmake_minimum_required(VERSION ${MINIMUM_CMAKE_VERSION})

project(Device VERSION ${PROJECT_VERSION} LANGUAGES CXX)

set(cxx_DIR "${SHARE_LIB_DIR}/cmake")

if(NOT TARGET cxx::cxx)
    find_package(cxx CONFIG REQUIRED)
endif()

add_library(${PROJECT_NAME} SHARED
  include/Device/device_def.h
  include/Device/endpoint.h
  include/Device/ideviceplugin.h src/Device/ideviceplugin.cpp
  include/Device/device.h
  include/Device/deviceset.h
  include/Device/devicepluginset.h src/Device/devicepluginset.cpp
)

add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

target_compile_definitions(${PROJECT_NAME}
  PRIVATE
    PLUMA_STATIC
)

target_include_directories(${PROJECT_NAME}
  PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/include/${PROJECT_NAME}
    ${CMAKE_CURRENT_LIST_DIR}/src/${PROJECT_NAME}
    ${CMAKE_CURRENT_LIST_DIR}/../${SHARE_DIR}/include
    ${CMAKE_CURRENT_LIST_DIR}/../Pluma-1.1/include
)

target_link_libraries(${PROJECT_NAME}
  PUBLIC
    $<BUILD_INTERFACE:Pluma>
    $<INSTALL_INTERFACE:$<$<CONFIG:Debug>:Plumad>>
    $<INSTALL_INTERFACE:$<$<NOT:$<CONFIG:Debug>>:Pluma>>
    cxx::cxx
)

include(${CMAKE_CURRENT_LIST_DIR}/../cmake/GenerateConfigPackage.cmake)
generate_config_package()
