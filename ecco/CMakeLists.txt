cmake_minimum_required(VERSION 3.15)
project(ecco LANGUAGES C CXX)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

set(SOURCES
    mmlib/src/Scheduler.cpp
    src/CEccoFuncs.cpp
    src/command/CEccoBaseCommand.cpp
    src/command/CEccoClientCommand.cpp
    src/command/CEccoServerCommand.cpp
    src/command/commands.cpp
    src/config/CConfig.cpp
    src/dllapi.cpp
    src/engine_api.cpp
    src/hud/CHudNumber.cpp
    src/hud/hud.cpp
    src/h_export.cpp
    src/lang/lang.cpp
    src/menu/executor/CBaseEccoExecutor.cpp
    src/menu/executor/CEccoBackExecutor.cpp
    src/menu/executor/CEccoScriptExecutor.cpp
    src/menu/executor/CEccoTextMenuExecutor.cpp
    src/menu/MenuParser.cpp
    src/meta_api.cpp
    src/meta_utility.cpp
    src/plugin/hook.cpp
    src/plugin/plugin_system.cpp
    src/scripts/CEccoScriptItem.cpp
    src/scripts/CEccoScriptSystem.cpp
    src/scripts/script_system.cpp
    src/scripts/tcl_loader.cpp
    src/sdk_util.cpp
    src/storage/CPlayerStorageItem.cpp
    src/storage/Storage.cpp
    src/timer/Timer.cpp
)
if(WIN32)
    list(APPEND SOURCES src/dllmain.cpp)
endif()

set(INCLUDE_DIRS
    ./metamod/hlsdk/common
    ./metamod/hlsdk/dlls
    ./metamod/hlsdk/pm_shared
    ./metamod/hlsdk/engine
    ./mmlib/src
    ./tcl/build/include
    ./include
    ./src
    ./
    ${META_INCLUDE}
)
include_directories(${INCLUDE_DIRS})

if(UNIX)
	set(CMAKE_CXX_VISIBILITY_PRESET hidden)
	set(CMAKE_VISIBILITY_INLINES_HIDDEN ON)
endif()

set(COMMON_DEFINES ${META_VERSION})
if(WIN32)
    list(APPEND COMMON_DEFINES WIN32 _WINDOWS PLATFORM_WINDOWS)
elseif(UNIX)
    list(APPEND COMMON_DEFINES LINUX PLATFORM_LINUX _GNU_SOURCE linux)
endif()
add_compile_definitions(${COMMON_DEFINES})

add_library(${PROJECT_NAME} SHARED ${SOURCES})

if(WIN32)
    target_compile_options(${PROJECT_NAME} PRIVATE /W3 /SDL /MDd /permissive-)
    set_target_properties(${PROJECT_NAME} PROPERTIES
        LINK_FLAGS "/DEF:${CMAKE_CURRENT_SOURCE_DIR}/exports.def"
        SUFFIX ".dll"
    )
    target_link_libraries(${PROJECT_NAME} PRIVATE kernel32.lib user32.lib)
elseif(UNIX)
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -O0 -g)
    set_target_properties(${PROJECT_NAME} PROPERTIES
        VERSION 1.0 SOVERSION 1 SUFFIX ".so"
    )
    target_link_libraries(${PROJECT_NAME} PRIVATE dl)
endif()


set(DEST_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../build/addons/metamod/dlls)
if(WIN32)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND if not exist "${DEST_DIR}" mkdir "${DEST_DIR}"
        COMMAND copy /Y "$<TARGET_FILE:${PROJECT_NAME}>" "${DEST_DIR}/"
    )
elseif(UNIX)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND mkdir -p "${DEST_DIR}"
        COMMAND cp -f "$<TARGET_FILE:${PROJECT_NAME}>" "${DEST_DIR}/"
    )
endif()