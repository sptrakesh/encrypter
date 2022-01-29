cmake_minimum_required(VERSION 3.2 FATAL_ERROR)

project(encrypter)

# Enable C++20 features
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug)
endif()

# Find includes in the build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(OpenSSL REQUIRED)
set(OPENSSL_USE_STATIC_LIBS TRUE)

set(CMAKE_CXX_FLAGS_DEBUG "-g -O0")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

if (NOT CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror")
endif()

if (UNIX AND NOT APPLE)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fcoroutines")
endif(UNIX AND NOT APPLE)

set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_STATIC_RUNTIME ON)
set(Boost_USE_MULTITHREADED ON)
set(OPENSSL_USE_STATIC_LIBS TRUE)

find_package(Boost)
find_package(OpenSSL REQUIRED)

if(Boost_FOUND)
  include_directories(${Boost_INCLUDE_DIRS})
endif()

if (APPLE)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fcoroutines-ts")
  set(CMAKE_MACOSX_RPATH 1)
endif()
if (UNIX)
  include_directories(
    /opt/local/include
  )
  link_directories(
    /opt/local/lib
  )
endif()

IF(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
  set(CMAKE_INSTALL_PREFIX /opt/spt)
ENDIF(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)

add_subdirectory(src/log)
add_subdirectory(src/lib)
add_subdirectory(src/api)
add_subdirectory(src/cli)
add_subdirectory(src/shell)
add_subdirectory(src/service)

if (BUILD_TESTING)
  enable_testing()
  add_subdirectory(test/unit)
  add_subdirectory(test/integration)
endif()

