# @file CMakeLists.txt
# @brief Builds generic device state and owning device cache implementations.

cmake_minimum_required(VERSION ${MINIMUM_CMAKE_VERSION})

project(DeviceBase VERSION ${PROJECT_VERSION} LANGUAGES CXX)

set(cxx_DIR "${SHARE_LIB_DIR}/cmake")

if(NOT TARGET cxx::cxx)
    find_package(cxx CONFIG REQUIRED)
endif()

add_library(${PROJECT_NAME} SHARED
  include/DeviceBase/basedevice.h src/DeviceBase/basedevice.cpp
  include/DeviceBase/basedeviceset.h src/DeviceBase/basedeviceset.cpp
)

add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

target_include_directories(${PROJECT_NAME}
  PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/include/${PROJECT_NAME}
    ${CMAKE_CURRENT_LIST_DIR}/src/${PROJECT_NAME}
    ${CMAKE_CURRENT_LIST_DIR}/../${SHARE_DIR}/include
    ${CMAKE_CURRENT_LIST_DIR}/../Device/include
)

target_link_libraries(${PROJECT_NAME}
  PUBLIC
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
