cmake_minimum_required(VERSION 3.16.3 FATAL_ERROR)
project(
  elapsed
  VERSION 3.0.5
  LANGUAGES C)
set(PROJECT_DESCRIPTION "Measure elapsed time.")

# Generate compile_commands.json file
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
# Set RPATH only if it's not a system directory
list(FIND CMAKE_PLATFORM_IMPLICIT_LINK_DIRECTORIES
     "${CMAKE_INSTALL_PREFIX}/lib" isSystemDir)
if("${isSystemDir}" STREQUAL "-1")
  set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")
endif()

if(CMAKE_C_COMPILER_ID STREQUAL "MSVC")
  # /wd5105:
  # https://docs.microsoft.com/en-us/cpp/error-messages/compiler-warnings/c5105?view=msvc-160
  set(WARNING_FLAGS /W3 /wd5105)
else()
  set(WARNING_FLAGS
      -Wall
      -Wextra
      -Wstrict-prototypes
      -Wshadow
      -Wconversion
      -Wmissing-prototypes
      -Wno-unused-parameter
      -Wsign-conversion
      -Wno-unused-function)

  if(NOT CMAKE_C_COMPILER_ID STREQUAL "GNU")
    list(APPEND WARNING_FLAGS -Wno-gnu-designator -Wno-empty-translation-unit
         -Wno-gnu-statement-expression -Wno-nullability-extension)
  endif()
endif()

option(CI_BUILD "CI, extra flags will be set" OFF)
if(CI_BUILD)
  message(STATUS "CI build enabled")
  if(CMAKE_C_COMPILER_ID STREQUAL "MSVC")
    add_compile_options(/WX)
  else()
    add_compile_options(-Werror)
  endif()
endif()

add_library(elapsed src/os.c src/elapsed.c)
add_library(ELAPSED::elapsed ALIAS elapsed)

# merge request fix:
# https://gitlab.kitware.com/cmake/cmake/-/merge_requests/1799
if(CMAKE_VERSION VERSION_GREATER 3.12.0)
  # Hide functions by default.
  set_property(TARGET elapsed PROPERTY C_VISIBILITY_PRESET hidden)
  set_property(TARGET elapsed PROPERTY VISIBILITY_INLINES_HIDDEN ON)
else()
  # Fix for https://github.com/horta/almosthere/issues/3
  set(GCC_TOO_OLD TRUE)
endif()

# Write out elapsed/export.h to the current binary directory.
include(GenerateExportHeader)
generate_export_header(
  elapsed
  BASE_NAME
  ELAPSED
  EXPORT_MACRO_NAME
  ELAPSED_API
  EXPORT_FILE_NAME
  ${CMAKE_CURRENT_BINARY_DIR}/elapsed/export.h)

target_include_directories(
  elapsed INTERFACE $<INSTALL_INTERFACE:include>
                    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>)
target_include_directories(
  elapsed
  PUBLIC $<INSTALL_INTERFACE:include>
         $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
         $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
set_target_properties(elapsed PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(elapsed PROPERTIES SOVERSION ${PROJECT_VERSION_MAJOR})
target_compile_features(elapsed INTERFACE c_std_11)
target_compile_options(elapsed PRIVATE ${WARNING_FLAGS})

# Include file check macros honor CMAKE_REQUIRED_LIBRARIES
if(CMAKE_VERSION VERSION_GREATER 3.12.0)
  cmake_policy(SET CMP0075 NEW)
endif()

include(CheckFunctionExists)
include(CheckIncludeFile)
include(CheckLibraryExists)
include(CheckSymbolExists)

check_include_file(stdnoreturn.h HAVE_STDNORETURN_H)
target_compile_definitions(
  elapsed PRIVATE $<$<BOOL:${HAVE_STDNORETURN_H}>:HAVE_STDNORETURN_H>)

check_symbol_exists(gettimeofday sys/time.h HAVE_GETTIMEOFDAY)
target_compile_definitions(
  elapsed PRIVATE $<$<BOOL:${HAVE_GETTIMEOFDAY_H}>:HAVE_GETTIMEOFDAY>)

check_include_file(unistd.h HAVE_UNISTD_H)
target_compile_definitions(elapsed
                           PRIVATE $<$<BOOL:${HAVE_UNISTD_H}>:HAVE_UNISTD_H>)

check_include_file(windows.h HAVE_WINDOWS_H)
target_compile_definitions(elapsed
                           PRIVATE $<$<BOOL:${HAVE_WINDOWS_H}>:HAVE_WINDOWS_H>)

check_symbol_exists(timespec_get time.h HAVE_TIMESPEC_GET)
target_compile_definitions(
  elapsed PRIVATE $<$<BOOL:${HAVE_TIMESPEC_GET}>:HAVE_TIMESPEC_GET>)

check_symbol_exists(clock_gettime time.h HAVE_CLOCK_GETTIME)
target_compile_definitions(
  elapsed PRIVATE $<$<BOOL:${HAVE_CLOCK_GETTIME}>:HAVE_CLOCK_GETTIME>)

check_include_file(sys/time.h HAVE_SYS_TIME_H)
target_compile_definitions(
  elapsed PRIVATE $<$<BOOL:${HAVE_SYS_TIME_H}>:HAVE_SYS_TIME_H>)

if(NOT (HAVE_CLOCK_GETTIME OR HAVE_TIMESPEC_GET))
  message(WARNING "Neiher clock_gettime nor timespec_get could be found.")
endif()

install(
  TARGETS elapsed
  EXPORT elapsed-targets
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
  PUBLIC_HEADER DESTINATION include)
install(DIRECTORY src/elapsed/ DESTINATION include/elapsed)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/elapsed/export.h
        DESTINATION include/elapsed)
install(
  EXPORT elapsed-targets
  FILE elapsed-targets.cmake
  NAMESPACE ELAPSED::
  DESTINATION lib/cmake/elapsed)
include(CMakePackageConfigHelpers)
configure_package_config_file(
  elapsed-config.cmake.in ${CMAKE_CURRENT_BINARY_DIR}/elapsed-config.cmake
  INSTALL_DESTINATION lib/cmake/elapsed
  NO_SET_AND_CHECK_MACRO)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/elapsed-config.cmake
        DESTINATION lib/cmake/elapsed)

install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/README.md
              ${CMAKE_CURRENT_SOURCE_DIR}/LICENSE.md
        DESTINATION share/docs/${PROJECT_NAME})

enable_testing()
add_subdirectory(test)

set(CPACK_PACKAGE_NAME ${PROJECT_NAME})
set(CPACK_PACKAGE_VENDOR "Danilo Horta")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Elapsed time C library")
set(CPACK_PACKAGE_CONTACT "Danilo Horta")
set(CPACK_PACKAGE_VERSION_MAJOR ${PROJECT_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${PROJECT_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${PROJECT_VERSION_PATCH})
set(CPACK_RESOURCE_FILE_LICENSE ${CMAKE_CURRENT_SOURCE_DIR}/LICENSE.md)
set(CPACK_RESOURCE_FILE_README ${CMAKE_CURRENT_SOURCE_DIR}/README.md)
set(CPACK_OUTPUT_FILE_PREFIX ${CMAKE_CURRENT_BINARY_DIR}/package)
set(CPACK_VERBATIM_VARIABLES YES)
set(CPACK_PACKAGE_RELOCATABLE YES)
set(CPACK_MONOLITHIC_INSTALL YES)
include(CPack)
