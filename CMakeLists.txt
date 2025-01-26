cmake_minimum_required(VERSION 3.16)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
include(GetVersion)
include(VersionDescription)

# Parse version
get_version(VERSION)
project(QtPack3r VERSION "${VERSION}" LANGUAGES CXX)

find_package(Qt6 6.2 REQUIRED COMPONENTS Core Widgets)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (WIN32)
    set(app_icon_resource_windows "${CMAKE_CURRENT_SOURCE_DIR}/resources/win32/${CMAKE_PROJECT_NAME}.rc")
else ()
    set(app_icon_resource_windows "")
endif ()

qt_add_executable(${CMAKE_PROJECT_NAME}
        WIN32
        src/main.cpp
        src/mainwindow.cpp
        src/mainwindow.h
        src/pack3r_process_handler.cpp
        src/pack3r_process_handler.h
        src/qtpack3r_widget.cpp
        src/qtpack3r_widget.h
        src/pack3r_output_parser.cpp
        src/pack3r_output_parser.h
        src/qtpack3r_widget_ui.cpp
        src/qtpack3r_widget_path_utils.cpp
        src/qtpack3r_widget_connections.cpp
        src/dialog.cpp
        src/dialog.h
        src/preferences.cpp
        src/preferences.h
        ${app_icon_resource_windows}
        resources/QtPack3r.qrc

)

get_version_description(QTPACK3R_VERSION)

target_compile_definitions(${CMAKE_PROJECT_NAME} PRIVATE
        PROJECT_NAME="${CMAKE_PROJECT_NAME}"
        PROJECT_VERSION="${CMAKE_PROJECT_VERSION}"
        GIT_COMMIT_HASH="${QTPACKER_VERSION_COMMIT_HASH}"
        GIT_COMMIT_HASH_SHORT="${QTPACKER_VERSION_COMMIT_HASH_SHORT}")

target_link_libraries(${CMAKE_PROJECT_NAME}
        PRIVATE
        Qt::Core
        Qt::Widgets
)

include(GNUInstallDirs)

install(TARGETS ${CMAKE_PROJECT_NAME}
        BUNDLE  DESTINATION .
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

# qt_generate_deploy_app_script() is only available on 6.3+
if (Qt6_VERSION VERSION_GREATER_EQUAL "6.3")
    qt_generate_deploy_app_script(
            TARGET ${CMAKE_PROJECT_NAME}
            OUTPUT_SCRIPT deploy_script
            NO_UNSUPPORTED_PLATFORM_ERROR
    )

    install(SCRIPT ${deploy_script})
else()
    message(STATUS "Qt version is older than 6.3, skipping deploy script generation.")
endif()

# set startup project for Visual Studio
if(CMAKE_GENERATOR MATCHES "Visual Studio")
    set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${CMAKE_PROJECT_NAME})
endif()
