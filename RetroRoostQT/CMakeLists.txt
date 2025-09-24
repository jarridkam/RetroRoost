cmake_minimum_required(VERSION 3.16)
project(RetroRoostQT LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

if (WIN32)
    set(CMAKE_PREFIX_PATH "C:/Qt/6.9.2/mingw_64/lib/cmake" CACHE PATH "Qt6 path" FORCE)
elseif(APPLE)
    if (EXISTS "/opt/homebrew/opt/qt")
        list(APPEND CMAKE_PREFIX_PATH "/opt/homebrew/opt/qt/lib/cmake")
    elseif (EXISTS "/usr/local/opt/qt")
        list(APPEND CMAKE_PREFIX_PATH "/usr/local/opt/qt/lib/cmake")
    endif()
endif()

find_package(Qt6 REQUIRED COMPONENTS
        Core
        Gui
        Qml
        Quick
        QuickControls2
        Sql
)

add_executable(RetroRoostQT
        main.cpp
        Resources/qml.qrc
        src/DatabaseManager.cpp
        src/DatabaseManager.h
        src/User/User.h
        src/User/UserRepository.cpp
        src/User/UserRepository.h
)

target_link_libraries(RetroRoostQT PRIVATE
        Qt6::Core
        Qt6::Gui
        Qt6::Qml
        Qt6::Quick
        Qt6::QuickControls2
        Qt6::Sql
)

# --- Windows only: run windeployqt after build ---
if (WIN32)
    add_custom_command(TARGET RetroRoostQT POST_BUILD
            COMMAND "${CMAKE_PREFIX_PATH}/../../bin/windeployqt.exe"
            --qmldir "${CMAKE_SOURCE_DIR}/QML"
            "$<TARGET_FILE:RetroRoostQT>"
            COMMENT "Running windeployqt to deploy Qt DLLs and QML modules"
    )
endif()
