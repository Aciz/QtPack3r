# This is used by the build pipelines atm to get the current project version
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
include(GetVersion)

get_version(VERSION)
message("${VERSION}")
