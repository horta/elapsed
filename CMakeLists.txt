cmake_minimum_required(VERSION 3.5.1 FATAL_ERROR)

# Generate compile_commands.json file
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Fetch version from header
set(VERSION_REGEX "#define ELAPSED_VERSION[ \t]+\"(.+)\"")
file(STRINGS "${CMAKE_CURRENT_SOURCE_DIR}/include/elapsed/elapsed.h"
  VERSION_STRING REGEX ${VERSION_REGEX})
string(REGEX REPLACE ${VERSION_REGEX} "\\1" VERSION_STRING "${VERSION_STRING}")

project(elapsed LANGUAGES C VERSION ${VERSION_STRING})
set(PROJECT_DESCRIPTION "Measure elapsed time.")

if(${CMAKE_C_COMPILER_ID} MATCHES "MSVC")
    set(WARNING_FLAGS /W3)
else()
    set(WARNING_FLAGS -Wall -Wextra -pedantic -Wstrict-prototypes -Wshadow
        -Wconversion -Wmissing-prototypes -Wno-unused-parameter -Wsign-conversion
        -Wno-unused-function # TODO: added only to please clangd for now
        -Wno-empty-translation-unit # TODO: added only to please clangd for now
    )
endif()

add_library(elapsed INTERFACE)
add_library(ELAPSED::elapsed ALIAS elapsed)
target_include_directories(elapsed
  INTERFACE
    $<INSTALL_INTERFACE:include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)
target_link_libraries(elapsed INTERFACE $<$<BOOL:${UNIX}>:m>)

install(TARGETS elapsed EXPORT elapsed-targets)
install(DIRECTORY include/ DESTINATION include)

# Export the targets to a script
install(EXPORT elapsed-targets
    FILE elapsed-targets.cmake
    NAMESPACE ELAPSED::
    DESTINATION lib/cmake/elapsed
)
include(CMakePackageConfigHelpers)
configure_package_config_file(elapsed-config.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/elapsed-config.cmake
    INSTALL_DESTINATION lib/cmake/elapsed
    NO_SET_AND_CHECK_MACRO
)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/elapsed-config.cmake
    DESTINATION lib/cmake/elapsed
)

enable_testing()
add_subdirectory(test)
