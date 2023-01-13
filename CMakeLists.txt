cmake_minimum_required(VERSION 3.16.3 FATAL_ERROR)
project(
  elapsed
  VERSION 3.0.9
  LANGUAGES C)
set(PROJECT_DESCRIPTION "Measure elapsed time.")

set(ELAPSED_MAIN_PROJECT OFF)
if(CMAKE_SOURCE_DIR STREQUAL PROJECT_SOURCE_DIR)
  set(ELAPSED_MAIN_PROJECT ON)
endif()

if(ELAPSED_MAIN_PROJECT)
  set(ELAPSED_BUILD_TESTS_DEFAULT ON)
else()
  set(ELAPSED_BUILD_TESTS_DEFAULT OFF)
endif()

option(ELAPSED_BUILD_TESTS "Build the unit tests"
       ${ELAPSED_BUILD_TESTS_DEFAULT})
message(STATUS "ELAPSED_MAIN_PROJECT: " ${ELAPSED_MAIN_PROJECT})
message(STATUS "ELAPSED_BUILD_TESTS: " ${ELAPSED_BUILD_TESTS})

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_EXTENSIONS OFF)

include(cmake/sanitizers.cmake)
include(cmake/warnings.cmake)

add_library(elapsed elapsed.c timespec.c sleep.c fatal.c)
add_library(ELAPSED::elapsed ALIAS elapsed)
if (NOT BUILD_SHARED_LIBS)
  target_compile_definitions(elapsed PUBLIC -DELAPSED_STATIC_DEFINE)
endif()


target_include_directories(
  elapsed
  PUBLIC $<INSTALL_INTERFACE:include>
         $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
set_target_properties(elapsed PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(elapsed PROPERTIES SOVERSION ${PROJECT_VERSION_MAJOR})
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
install(DIRECTORY include/elapsed DESTINATION include/elapsed)
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

if(ELAPSED_BUILD_TESTS)
  enable_testing()
  macro(inject_dll_to_please_windows test_name)
    file(TO_CMAKE_PATH "$ENV{PATH}" PATH)
    list(APPEND PATH $<TARGET_FILE_DIR:ELAPSED::elapsed>)
    string(REPLACE ";" "\\;" PATH "${PATH}")
    set_tests_properties(${test_name} PROPERTIES ENVIRONMENT "PATH=${PATH}")
  endmacro()

  add_executable(test_elapsed test_elapsed.c)
  target_link_libraries(test_elapsed PRIVATE ELAPSED::elapsed)
  target_compile_options(test_elapsed PRIVATE ${WARNING_FLAGS})
  add_test(NAME elapsed COMMAND test_elapsed)
  inject_dll_to_please_windows(elapsed)
endif()
