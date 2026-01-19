cmake_minimum_required(VERSION 3.16)

project(EKNMusic VERSION 1.0.0 LANGUAGES CXX)

# C++ Standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Qt Configuration
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Find Qt6
find_package(Qt6 REQUIRED COMPONENTS
    Core
    Widgets
    Network
    Sql
    Multimedia
)

# Include directories
include_directories(
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_BINARY_DIR}
)

# Source files
set(SOURCES
    src/main.cpp
    src/ui/loginwindow.cpp
    src/ui/mainwindow.cpp
    src/ui/likedsongs.cpp
    src/ui/searchpage.cpp
    src/ui/downloadedpage.cpp
    src/ui/playerwidget.cpp
    src/ui/playerpage.cpp
    src/models/track.cpp
    src/models/playlistdata.cpp
    src/services/playerservice.cpp
    src/services/musicstorageservice.cpp
    src/services/metadataextractor.cpp
)

set(HEADERS
    src/ui/loginwindow.h
    src/ui/mainwindow.h
    src/ui/likedsongs.h
    src/ui/searchpage.h
    src/ui/downloadedpage.h
    src/ui/playerwidget.h
    src/ui/playerpage.h
    src/models/track.h
    src/models/playlistdata.h
    src/services/playerservice.h
    src/services/musicstorageservice.h
    src/services/metadataextractor.h
)

set(UI_FILES
    # Add your .ui files here
    # src/ui/mainwindow.ui
)

set(RESOURCES
    resources.qrc
)

# Platform-specific settings
if(WIN32)
    # Windows specific settings
    # TODO: Add app icon in future iterations
    # set(APP_ICON_RESOURCE_WINDOWS "${CMAKE_SOURCE_DIR}/platform/windows/app.rc")
    # set(SOURCES ${SOURCES} ${APP_ICON_RESOURCE_WINDOWS})

    # Enable console for debug builds on Windows
    if(CMAKE_BUILD_TYPE MATCHES Debug)
        set(CMAKE_WIN32_EXECUTABLE OFF)
    else()
        set(CMAKE_WIN32_EXECUTABLE ON)
    endif()
endif()

if(APPLE)
    # macOS specific settings
    # TODO: Add app icon in future iterations
    # set(MACOSX_BUNDLE_ICON_FILE icon.icns)
    # set(APP_ICON_MACOSX "${CMAKE_SOURCE_DIR}/platform/macos/icon.icns")
    # set_source_files_properties(${APP_ICON_MACOSX} PROPERTIES
    #     MACOSX_PACKAGE_LOCATION "Resources")
    # set(SOURCES ${SOURCES} ${APP_ICON_MACOSX})
endif()

if(UNIX AND NOT APPLE)
    # Linux specific settings
endif()

# Create executable
if(APPLE)
    add_executable(${PROJECT_NAME} MACOSX_BUNDLE
        ${SOURCES}
        ${HEADERS}
        ${UI_FILES}
        ${RESOURCES}
    )

    # macOS bundle properties
    set_target_properties(${PROJECT_NAME} PROPERTIES
        MACOSX_BUNDLE_INFO_PLIST "${CMAKE_SOURCE_DIR}/platform/macos/Info.plist.in"
        MACOSX_BUNDLE_BUNDLE_NAME ${PROJECT_NAME}
        MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
        MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION}
        MACOSX_BUNDLE_ICON_FILE icon.icns
    )
elseif(WIN32)
    add_executable(${PROJECT_NAME} WIN32
        ${SOURCES}
        ${HEADERS}
        ${UI_FILES}
        ${RESOURCES}
    )
else()
    add_executable(${PROJECT_NAME}
        ${SOURCES}
        ${HEADERS}
        ${UI_FILES}
        ${RESOURCES}
    )
endif()

# Link Qt libraries
target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt6::Core
    Qt6::Widgets
    Qt6::Network
    Qt6::Sql
    Qt6::Multimedia
)

# Platform-specific libraries
if(WIN32)
    # Windows specific libraries
    target_link_libraries(${PROJECT_NAME} PRIVATE ws2_32)
endif()

if(APPLE)
    # macOS specific frameworks
    target_link_libraries(${PROJECT_NAME} PRIVATE
        "-framework CoreFoundation"
        "-framework Security"
    )
endif()

# Compiler warnings
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic)
endif()

# Install rules
install(TARGETS ${PROJECT_NAME}
    BUNDLE DESTINATION .
    RUNTIME DESTINATION bin
)

# Qt deployment (optional, for packaging)
if(Qt6_FOUND)
    if(WIN32)
        # Windows deployment will be handled by windeployqt
    elseif(APPLE)
        # macOS deployment will be handled by macdeployqt
    else()
        # Linux deployment
        install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)
    endif()
endif()

# Testing
option(BUILD_TESTS "Build tests" OFF)

if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

# Print configuration
message(STATUS "")
message(STATUS "Configuration Summary:")
message(STATUS "  Project: ${PROJECT_NAME}")
message(STATUS "  Version: ${PROJECT_VERSION}")
message(STATUS "  Build Type: ${CMAKE_BUILD_TYPE}")
message(STATUS "  C++ Standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "  Qt Version: ${Qt6_VERSION}")
message(STATUS "  Build Tests: ${BUILD_TESTS}")
message(STATUS "")
