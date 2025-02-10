# Creates a package ready for release
# - Windows: runs windeployqt to package the required dependencies
# - Linux: bundles .desktop file and an icon to a directory structure following freedesktop specification

# noop
if(NOT INPUT)
    return()
endif()

if(NOT OUTPUT)
    message(FATAL_ERROR "OUTPUT is not specified")
endif()

string(REPLACE " " ";" ASSETS ${INPUT})
# glob so we evaluate the input expressions
file(GLOB GLOBBED_ASSETS RELATIVE ${CMAKE_SOURCE_DIR} ${ASSETS})

if (WIN32)
    # TODO: figure this out...
    #[[
    execute_process(COMMAND windeployqt
            --release
            --force
            --no-translations
            --no-compiler-runtime
            --no-opengl-sw
            --no-system-d3d-compiler
            --skip-plugin-types generic,iconengines,imageformats,networkinformation
            "${INPUT}" # the input on Windows is simply the executable file
    )
    ]]
else ()
    set(DESKTOP_FILE_PATH "${CMAKE_BINARY_DIR}/applications")
    set(ICON_FILE_PATH "${CMAKE_BINARY_DIR}/icons/hicolor/48x48/apps")

    file(MAKE_DIRECTORY "${DESKTOP_FILE_PATH}")
    file(MAKE_DIRECTORY "${ICON_FILE_PATH}")

    # CMake offers no way to get an item from list via pattern,
    # so we need to create new lists and use filtering...
    set(DESKTOP_FILES ${GLOBBED_ASSETS})
    set(ICON_FILES ${GLOBBED_ASSETS})

    list(FILTER DESKTOP_FILES INCLUDE REGEX ".*\\.desktop$")
    list(FILTER ICON_FILES INCLUDE REGEX ".*\\.png$")

    list(GET DESKTOP_FILES 0 DESKTOP_FILE_IN)
    list(GET ICON_FILES 0 ICON_FILE_IN)

    if (DESKTOP_FILES)
        file(COPY ${DESKTOP_FILE_IN} DESTINATION ${DESKTOP_FILE_PATH})
    else ()
        message(FATAL_ERROR "Unable to locate .desktop file for packing!")
    endif ()

    if (ICON_FILES)
        file(COPY ${ICON_FILE_IN} DESTINATION ${ICON_FILE_PATH})
    else ()
        message(FATAL_ERROR "Unable to locate icon file for packing!")
    endif ()

    file(GLOB GLOBBED_ASSETS
            "${CMAKE_BINARY_DIR}/applications/*"
            "${CMAKE_BINARY_DIR}/icons/hicolor/48x48/apps/*"
            "${CMAKE_BINARY_DIR}/QtPack3r" # TODO: pass in as an argument?
    )
endif ()

execute_process(COMMAND ${CMAKE_COMMAND} -E tar "cf" ${OUTPUT} --format=zip -- ${GLOBBED_ASSETS})
