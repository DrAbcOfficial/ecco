cmake_minimum_required(VERSION 3.16 FATAL_ERROR)
project(base LANGUAGES C CXX)


set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

set(GCC_COVERAGE_COMPILE_FLAGS "-m32")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}")
set(CMAKE_POSITION_INDEPENDENT_CODE ON) 

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0 -g -ggdb")
    set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -O0 -g -ggdb")
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O2 -flto")
    set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -O2 -flto")
    set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -flto")
endif()

set(SOURCE_FILES
    plugin.cpp
)
set(HEADER_FILES
    sdk_api.h
)

add_library(${PROJECT_NAME} SHARED
    ${SOURCE_FILES}
    ${HEADER_FILES}
)

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

target_compile_definitions(${PROJECT_NAME} PRIVATE
    BASE_EXPORTS 
    $<$<CONFIG:Debug>:_DEBUG>
    $<$<CONFIG:Release>:NDEBUG>
)

target_include_directories(${PROJECT_NAME} PRIVATE
    ./mmlib/include
    ./mmlib/include/metamod
    ./mmlib/include/hlsdk/engine
    ./mmlib/include/hlsdk/dlls
    ./mmlib/include/hlsdk/pm_shared
    ./mmlib/include/hlsdk/common
    ../ecco/include
)

target_compile_options(${PROJECT_NAME} PRIVATE
    -Wall
    -Wextra
    -fstack-protector-strong
    -Wuninitialized
    -Wmaybe-uninitialized
    -Wpedantic
)

target_link_options(${PROJECT_NAME} PRIVATE
    $<$<CONFIG:Debug>:--no-undefined>
)