# @file CMakeLists.txt
# @brief Configures shared build settings and adds utility, device, UI, and application targets.

set(MINIMUM_CMAKE_VERSION 3.16)
set(PROJECT_VERSION 1.0.0)

cmake_minimum_required(VERSION ${MINIMUM_CMAKE_VERSION})

project(VisualRobot VERSION ${PROJECT_VERSION} LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_DEBUG_POSTFIX "d")

set(SHARE_DIR "share")
set(SHARE_LIB_DIR "${CMAKE_CURRENT_LIST_DIR}/../${SHARE_DIR}")
set(THIRDPARTY_DIR "")

set(CMAKE_INSTALL_PREFIX "${SHARE_LIB_DIR}"
  CACHE PATH "Installation directory." FORCE)

if(WIN32)
  message(STATUS "current is the WIN32 platform.")
  set(THIRDPARTY_DIR "${CMAKE_CURRENT_LIST_DIR}/3rdparty/WIN32")
elseif(APPLE)
  message(STATUS "current is the APPLE platform.")
  set(THIRDPARTY_DIR "${CMAKE_CURRENT_LIST_DIR}/3rdparty/APPLE")
elseif(ANDROID)
  message(STATUS "current is the ANDROID platform.")
  set(THIRDPARTY_DIR "${CMAKE_CURRENT_LIST_DIR}/3rdparty/ANDROID")
elseif(UNIX)
  message(STATUS "current is the UNIX platform.")
  set(THIRDPARTY_DIR "${CMAKE_CURRENT_LIST_DIR}/3rdparty/UNIX")
else()
  message(FATAL_ERROR "unknown os platform.")
endif()

add_subdirectory(Pluma-1.1)
add_subdirectory(cxx)
add_subdirectory(Qtx)

add_subdirectory(Device)
add_subdirectory(DeviceBase)
add_subdirectory(AgvRobot)
add_subdirectory(HumanoidRobot)

add_subdirectory(Ui)
add_subdirectory(UiBase)
add_subdirectory(Home)
add_subdirectory(RemoteControl)

add_subdirectory(Application)