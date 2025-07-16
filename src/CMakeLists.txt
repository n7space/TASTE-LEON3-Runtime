add_library(Leon3Runtime STATIC)

target_sources(Leon3Runtime
  PUBLIC       Perfmon.h
               CbiPartitionApi.h
               Hal.h
  PRIVATE      Perfmon.c
               CbiPartitionApi.c
               Hal.c)

target_include_directories(Leon3Runtime
  PRIVATE   ${CMAKE_SOURCE_DIR}/src
            ${CMAKE_SOURCE_DIR}/test/mock)

add_format_target(Leon3Runtime)