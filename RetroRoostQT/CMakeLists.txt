cmake_minimum_required(VERSION 3.16)
project(RetroRoostQT LANGUAGES CXX)

# -----------------------------------------------------------------------------
#  C++ and Qt configuration
# -----------------------------------------------------------------------------
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# -----------------------------------------------------------------------------
#  Qt prefix path (platform-specific)
# -----------------------------------------------------------------------------
if (WIN32)
    # Adjust to your installed Qt version / kit
    set(CMAKE_PREFIX_PATH "C:/Qt/6.9.2/mingw_64/lib/cmake" CACHE PATH "Qt6 path" FORCE)
elseif(APPLE)
    if (EXISTS "/opt/homebrew/opt/qt")
        list(APPEND CMAKE_PREFIX_PATH "/opt/homebrew/opt/qt/lib/cmake")
    elseif (EXISTS "/usr/local/opt/qt")
        list(APPEND CMAKE_PREFIX_PATH "/usr/local/opt/qt/lib/cmake")
    endif()
endif()

# -----------------------------------------------------------------------------
#  Qt modules
# -----------------------------------------------------------------------------
find_package(Qt6 REQUIRED COMPONENTS
        Core
        Gui
        Qml
        Quick
        QuickControls2
        Sql
)

# -----------------------------------------------------------------------------
#  Project source structure
# -----------------------------------------------------------------------------
set(SRC_DIR "${CMAKE_SOURCE_DIR}/src")

set(SOURCES
        main.cpp
        Resources/qml.qrc

        ${SRC_DIR}/core/DatabaseManager.cpp
        ${SRC_DIR}/core/DatabaseManager.h

        ${SRC_DIR}/core/AuthorizationManager.cpp
        ${SRC_DIR}/core/AuthorizationManager.h

        ${SRC_DIR}/core/models/User.h

        ${SRC_DIR}/repositories/UserRepository.cpp
        ${SRC_DIR}/repositories/UserRepository.h
)

add_executable(RetroRoostQT ${SOURCES})

# -----------------------------------------------------------------------------
#  Link Qt libraries
# -----------------------------------------------------------------------------
target_link_libraries(RetroRoostQT PRIVATE
        Qt6::Core
        Qt6::Gui
        Qt6::Qml
        Qt6::Quick
        Qt6::QuickControls2
        Qt6::Sql
)

# -----------------------------------------------------------------------------
#  Platform-specific post-build steps
# -----------------------------------------------------------------------------
if (WIN32)
    # --- Deploy Qt DLLs ---
    add_custom_command(TARGET RetroRoostQT POST_BUILD
            COMMAND "${CMAKE_PREFIX_PATH}/../../bin/windeployqt.exe"
            --qmldir "${CMAKE_SOURCE_DIR}/QML"
            "$<TARGET_FILE:RetroRoostQT>"
            COMMENT "Running windeployqt to deploy Qt DLLs and QML modules"
    )

    # --- Copy PostgreSQL runtime libraries ---
    set(POSTGRESQL_BIN_DIR "C:/Program Files/PostgreSQL/18/bin")

    file(GLOB POSTGRESQL_DLLS
            "${POSTGRESQL_BIN_DIR}/libpq.dll"
            "${POSTGRESQL_BIN_DIR}/libssl-3-x64.dll"
            "${POSTGRESQL_BIN_DIR}/libcrypto-3-x64.dll"
            "${POSTGRESQL_BIN_DIR}/libiconv-2.dll"
            "${POSTGRESQL_BIN_DIR}/libintl-9.dll"
    )

    foreach(dll ${POSTGRESQL_DLLS})
        if(EXISTS "${dll}")
            add_custom_command(TARGET RetroRoostQT POST_BUILD
                    COMMAND ${CMAKE_COMMAND} -E copy_if_different
                    "${dll}" "$<TARGET_FILE_DIR:RetroRoostQT>"
                    COMMENT "Copying ${dll} to output folder"
            )
        endif()
    endforeach()
endif()

# -----------------------------------------------------------------------------
#  (Optional) macOS/Linux deploy hints
# -----------------------------------------------------------------------------
if(APPLE)
    message(STATUS "Use macdeployqt to bundle frameworks after building.")
elseif(UNIX AND NOT APPLE)
    message(STATUS "For Linux deployment, ensure libpq and Qt6 are in LD_LIBRARY_PATH.")
endif()
