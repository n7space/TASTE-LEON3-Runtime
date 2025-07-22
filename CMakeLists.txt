cmake_minimum_required(VERSION 3.13 FATAL_ERROR)
project(TASTE-Leon3-Runtime VERSION 1.0.0 LANGUAGES C CXX)

set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/build/bin)
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/build/lib)
set(TESTS_OUTPUT_PATH ${CMAKE_BINARY_DIR}/build/tests)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake-tools)

include(MessageColors)
include(LoggingHelper)

add_library(common_build_options INTERFACE)
include(CommonBuildOptions)
include(Sanitizers)

option(TASTE_RUNTIME_COMMON_WARNINGS_AS_ERRORS
       "Treat compiler wargnings as errors"
       TRUE)

if(OPTIONS_WARNINGS_AS_ERRORS)
    log_option_enabled("warnings as errors")
    set(CLANG_WARNINGS ${CLANG_WARNINGS} -Werror)
endif()

set(CLANG_WARNINGS -Wall
                   -Wextra
                   -Wpedantic
                   -Wshadow
                   -Wcast-align
                   -Wunused
                   -Wsign-conversion
                   -Wnull-dereference
                   -Wdouble-promotion
                   -Wbad-function-cast
                   -Wno-unused-parameter
                   -Wno-array-bounds
                   -Wformat=2)

set(GCC_WARNINGS ${CLANG_WARNINGS}
                   -Wmisleading-indentation
                   -Wduplicated-cond
                   -Wduplicated-branches
                   -Wlogical-op)

if(CMAKE_C_COMPILER_ID STREQUAL "Clang")
    set(PROJECT_WARNINGS ${CLANG_WARNINGS})
elseif(CMAKE_C_COMPILER_ID STREQUAL "GNU")
    set(PROJECT_WARNINGS ${GCC_WARNINGS})
endif()

file(GLOB_RECURSE MOCK_DIR "test/mock/*.c" "test/mock/*.h")

set_source_files_properties(
    MOCK_DIR
    PROPERTIES APPEND_STRING PROPERTY COMPILE_FLAGS " -w"
)

target_compile_options(common_build_options INTERFACE ${PROJECT_WARNINGS})
add_compile_options(${PROJECT_WARNINGS})

include(CppCheck)
include(ClangTidy)
include(ClangFormat)
include(Doxygen)

add_subdirectory(src)