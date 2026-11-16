# @file CMakeLists.txt
# @brief Builds common C++ utilities with the spdlog backend.

cmake_minimum_required(VERSION ${MINIMUM_CMAKE_VERSION})

project(cxx VERSION ${PROJECT_VERSION} LANGUAGES CXX)

set(spdlog_DIR "${THIRDPARTY_DIR}/spdlog/lib/cmake/spdlog")
find_package(spdlog CONFIG REQUIRED)

add_library(${PROJECT_NAME} SHARED
  include/cxx/cxx_def.h
  include/cxx/guard_thread.hpp
  include/cxx/singleton.hpp
  include/cxx/cxx_kit.h src/cxx/cxx_kit.cpp
  include/cxx/byte_stream.h include/cxx/byte_stream.cpp
  include/cxx/date_time.h src/cxx/date_time.cpp
  include/cxx/string_util.h include/cxx/string_util.cpp
  include/cxx/simple_vector.h include/cxx/simple_vector.cpp
  include/cxx/spdlog_guard.h src/cxx/spdlog_guard.cpp
  include/cxx/logger.h src/cxx/logger.cpp
  include/cxx/item.h src/cxx/item.cpp
  include/cxx/list_item.h src/cxx/list_item.cpp
)

add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

target_include_directories(${PROJECT_NAME}
  PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/include/${PROJECT_NAME}
    ${CMAKE_CURRENT_LIST_DIR}/src/${PROJECT_NAME}
    ${CMAKE_CURRENT_LIST_DIR}/../${SHARE_DIR}/include
)

target_link_libraries(${PROJECT_NAME}
  PUBLIC
    spdlog::spdlog
)

include(${CMAKE_CURRENT_LIST_DIR}/../cmake/GenerateConfigPackage.cmake)
generate_config_package()