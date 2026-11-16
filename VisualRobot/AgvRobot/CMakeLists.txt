# @file CMakeLists.txt
# @brief Builds and deploys the AGV device plugin using Pluma and DeviceBase.

cmake_minimum_required(VERSION ${MINIMUM_CMAKE_VERSION})

project(AgvRobot VERSION ${PROJECT_VERSION} LANGUAGES CXX)

set(cxx_DIR "${SHARE_LIB_DIR}/cmake")
set(DeviceBase_DIR "${SHARE_LIB_DIR}/cmake")

if(NOT TARGET cxx::cxx)
    find_package(cxx CONFIG REQUIRED)
endif()

if(NOT TARGET DeviceBase::DeviceBase)
    find_package(DeviceBase CONFIG REQUIRED)
endif()

add_library(${PROJECT_NAME} SHARED
  src/AgvRobot/connector.cpp
  include/AgvRobot/agvrobot.h src/AgvRobot/agvrobot.cpp
  include/AgvRobot/agvrobotset.h src/AgvRobot/agvrobotset.cpp
  include/AgvRobot/agvrobotdevice.h src/AgvRobot/agvrobotdevice.cpp
)

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
    ${CMAKE_CURRENT_LIST_DIR}/../Device/include
)

target_link_libraries(${PROJECT_NAME}
  PUBLIC
    $<BUILD_INTERFACE:Pluma>
    $<INSTALL_INTERFACE:$<$<CONFIG:Debug>:Plumad>>
    $<INSTALL_INTERFACE:$<$<NOT:$<CONFIG:Debug>>:Pluma>>
    cxx::cxx
    DeviceBase::DeviceBase
)

# @brief Deploy the built device plugin to the application plugin directory.
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "$<TARGET_FILE:${PROJECT_NAME}>"
        "${CMAKE_CURRENT_BINARY_DIR}/../Application/plugins/device"
    VERBATIM
)

include(GNUInstallDirs)

install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)