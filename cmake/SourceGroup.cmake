if(MSVC)
    set(SRC_PATH "src")
    file(GLOB_RECURSE ALL_SOURCES "${SRC_PATH}/*.*")

    get_filename_component(SRC_FULLPATH ${SRC_PATH} ABSOLUTE)

    foreach (SRCFILE ${ALL_SOURCES})
        get_filename_component(FILE_EXT ${SRCFILE} EXT)

        if(FILE_EXT STREQUAL ".cpp")
            source_group("Source Files" FILES ${SRCFILE})
        elseif(FILE_EXT STREQUAL ".h")
            source_group("Header Files" FILES ${SRCFILE})
        endif()
    endforeach ()

    # Add resources too so they are visible in the solution
    set(RESOURCE_PATH "resources")
    file(GLOB_RECURSE RESOURCES "${RESOURCE_PATH}/*.*" )
    source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/resources" PREFIX "Resources" FILES ${RESOURCES})
endif()
