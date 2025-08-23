cmake_minimum_required(VERSION 3.16 FATAL_ERROR)

project(ecco LANGUAGES C CXX)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(GCC_COVERAGE_COMPILE_FLAGS "-m32")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}")
set(CMAKE_POSITION_INDEPENDENT_CODE ON) 

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0 -g -ggdb")
    set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -O0 -g -ggdb")
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -flto") 
    set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -O3 -flto")
    set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -flto")
endif()

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

set(HEADER_FILES
    include/api/HookDef.h
    include/api/IEccoBaseExcutor.h
    include/api/IEccoFuncs.h
    include/api/IEccoPlugin.h
    include/api/IEccoScriptSystem.h
    include/api/IPlayerStorageItem.h
    mmlib/src/Scheduler.h
    src/CEccoFuncs.h
    src/dlldef.h
    src/enginedef.h
    src/info_name.h
    src/meta_utility.h
    src/signatures.h
    src/command/CEccoBaseCommand.h
    src/command/CEccoClientCommand.h
    src/command/CEccoServerCommand.h
    src/command/command_system.h
    src/config/CConfig.h
    src/hud/CHudNumber.h
    src/hud/hud.h
    src/lang/lang.h
    src/menu/executor/CBaseEccoExecutor.h
    src/menu/executor/CEccoBackExecutor.h
    src/menu/executor/CEccoScriptExecutor.h
    src/menu/executor/CEccoTextMenuExecutor.h
    src/menu/MenuParser.h
    src/plugin/plugin_system.h
    src/scripts/CEccoScriptItem.h
    src/scripts/CEccoScriptSystem.h
    src/scripts/script_system.h
    src/scripts/tclDecls_dynamic.h
    src/scripts/tcl_dynamic.h
    src/scripts/tcl_loader.h
    src/storage/CPlayerStorageItem.h
    src/storage/Storage.h
    src/timer/Timer.h
)

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

set(COMMON_DEFINES ${META_VERSION})

if(WIN32)
    list(APPEND COMMON_DEFINES
        WIN32
        _WINDOWS
        _USRDLL
        PLATFORM_WINDOWS
        _CRT_SECURE_NO_WARNINGS
        $<$<CONFIG:Debug>:_DEBUG>
        $<$<CONFIG:Release>:NDEBUG> 
    )
elseif(UNIX)
    list(APPEND COMMON_DEFINES
        LINUX
        PLATFORM_LINUX
        _GNU_SOURCE
        linux
    )
endif()
add_compile_definitions(${COMMON_DEFINES})

add_library(${PROJECT_NAME} SHARED
    ${SOURCES}
    ${HEADER_FILES}
)

target_compile_options(${PROJECT_NAME} PRIVATE
    -Wall
    -Wextra
    -fstack-protector-strong
    -Wno-unused-parameter
    $<$<CONFIG:Debug>:-O0 -g>
    $<$<CONFIG:Release>:-O3 -flto>
)

set_target_properties(${PROJECT_NAME} PROPERTIES
    VERSION 1.0
    SOVERSION 1
    SUFFIX ".so"
    CXX_VISIBILITY_PRESET hidden
    VISIBILITY_INLINES_HIDDEN ON
)