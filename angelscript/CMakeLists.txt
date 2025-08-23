cmake_minimum_required(VERSION 3.16 FATAL_ERROR)
project(angelscript LANGUAGES C CXX)

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
    angelscript/angelscript.cpp
    plugin.cpp
)
set(HEADER_FILES
    angelscript/angelscript.hpp
    angelscript/angelscriptlib.h
)
add_library(${PROJECT_NAME} SHARED
    ${SOURCE_FILES}
    ${HEADER_FILES}
)
target_compile_definitions(${PROJECT_NAME} PRIVATE
    BASE_EXPORTS
    $<$<CONFIG:Debug>:_DEBUG>
    $<$<CONFIG:Release>:NDEBUG>
)
target_include_directories(${PROJECT_NAME} PRIVATE
    ./metamod/metamod
    ./metamod/hlsdk/engine
    ./metamod/hlsdk/dlls
    ./metamod/hlsdk/pm_shared
    ./metamod/hlsdk/common
    ./metamod/asext/include
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