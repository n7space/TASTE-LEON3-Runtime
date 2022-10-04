add_library(Leon3Runtime STATIC)
target_sources(Leon3Runtime
  PUBLIC       Perfmon.h
  PRIVATE      Perfmon.c)

add_format_target(Leon3Runtime)