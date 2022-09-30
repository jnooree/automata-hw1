cmake_minimum_required(VERSION 3.16 FATAL_ERROR) # Version for Ubuntu 20.04 LTS
project(automata-hw1 LANGUAGES CXX)

set(CMAKE_BUILD_TYPE Release CACHE STRING "Bulid type")
option(CODE_COVERAGE "Build for test coverage" OFF)

if(CMAKE_BUILD_TYPE STREQUAL "")
  set(CMAKE_BUILD_TYPE Release)
endif()

# Compile options
set(OPTIMIZATION_LEVEL "O3" CACHE STRING "Optimization level")
option(ENABLE_IPO "Do interprocedural optimization" ON)
option(ENABLE_ARCH_NATIVE "Use -march=native flag for optimization" ON)

add_compile_options(
  -fno-rtti -fno-exceptions -pedantic -Wall -Wextra -Wno-sign-compare)

if(CODE_COVERAGE)
  include(CTest)
  string(APPEND CMAKE_CXX_FLAGS "--coverage")
endif()

# Only do these if this is the main project, and not if it is included through
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
  set(CMAKE_CXX_STANDARD 17)
  set(CMAKE_CXX_EXTENSIONS ON)
  set(CMAKE_CXX_STANDARD_REQUIRED ON)
  set(CMAKE_POSITION_INDEPENDENT_CODE ON)
  set(CMAKE_INSTALL_RPATH_USE_LINK_PATH ON)

  set(GLOBAL_OPTFLAGS " -${OPTIMIZATION_LEVEL}")

  if(ENABLE_ARCH_NATIVE)
    string(APPEND GLOBAL_OPTFLAGS " -march=native")
  endif()

  string(APPEND CMAKE_CXX_FLAGS_RELEASE "${GLOBAL_OPTFLAGS}")
  string(APPEND CMAKE_CXX_FLAGS_RELWITHDEBINFO "${GLOBAL_OPTFLAGS}")

  if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_options(-O0)
    add_compile_definitions("DEBUG")

    set(CMAKE_DEBUG_POSTFIX ".debug")
    string(PREPEND CMAKE_EXECUTABLE_SUFFIX ".debug")
  elseif(ENABLE_IPO)
    set(CMAKE_INTERPROCEDURAL_OPTIMIZATION ON)
  endif()
endif()

add_subdirectory(src)

if(CODE_COVERAGE)
  add_subdirectory(test)
endif()
