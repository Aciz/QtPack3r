# FIXME: this is kinda dumb, we already do this in CMakeLists.txt
# read version from VERSION.txt
file(READ "${CMAKE_SOURCE_DIR}/VERSION.txt" version_content)

string(REGEX MATCH "VERSION_MAJOR ([0-9]+)" _major_match "${version_content}")
set(VERSION_MAJOR "${CMAKE_MATCH_1}")
string(REGEX MATCH "VERSION_MINOR ([0-9]+)" _minor_match "${version_content}")
set(VERSION_MINOR "${CMAKE_MATCH_1}")
string(REGEX MATCH "VERSION_PATCH ([0-9]+)" _patch_match "${version_content}")
set(VERSION_PATCH "${CMAKE_MATCH_1}")

message("${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}")
