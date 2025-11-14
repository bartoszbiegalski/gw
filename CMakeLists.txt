cmake_minimum_required(VERSION 3.16)
project(gw VERSION 0.3 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# compile options
if(MSVC)
    add_compile_options(/W4 /permissive-)
else()
    add_compile_options(-Wall -Wextra -Wpedantic -Wconversion -O3)
endif()

add_definitions(-DLIBXML_STATIC -DLIBZIP_NO_CRYPTO)

# vcpkg
if(NOT DEFINED CMAKE_TOOLCHAIN_FILE AND DEFINED ENV{VCPKG_ROOT})
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake"
        CACHE STRING "Vcpkg toolchain file")
    message(STATUS "Using vcpkg toolchain: ${CMAKE_TOOLCHAIN_FILE}")
endif()

# libxml2, libzip

if(MSVC)
    find_package(LibXml2 CONFIG REQUIRED)
    find_package(libzip CONFIG REQUIRED)
    set(LIBXML2_TARGET libxml2::libxml2)
    set(LIBZIP_TARGET libzip::libzip)
else()
    find_package(PkgConfig REQUIRED)    
    pkg_check_modules(LIBXML2 REQUIRED libxml-2.0)
    pkg_check_modules(LIBZIP REQUIRED libzip)

    set(LIBXML2_TARGET ${LIBXML2_LIBRARIES})
    set(LIBZIP_TARGET  ${LIBZIP_LIBRARIES})
endif()

# Pliki projektu
file(GLOB_RECURSE GW_LIBRARY_HEADERS 
    "benchmark/benchmark.h"
    "include/*.h"
)
file(GLOB_RECURSE GW_LIBRARY_SOURCES
    "benchmark/benchmark.cpp"
    "src/core/*.cpp"
    "src/io/*.cpp"
    "src/logic/*.cpp"
    "src/services/*.cpp"
    "src/utils/*.cpp"
)

# static library gw_lib
add_library(gw_lib STATIC ${GW_LIBRARY_HEADERS} ${GW_LIBRARY_SOURCES})
add_subdirectory(tests)

target_include_directories(gw_lib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/benchmark
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_BINARY_DIR}
)

# external 
if(WIN32)
    #costam
    target_include_directories(gw_lib PUBLIC 
        ${CMAKE_SOURCE_DIR}/external/cppuprofile/lib
    )
else()
    target_include_directories(gw_lib PUBLIC 
        ${CMAKE_SOURCE_DIR}/external/cppuprofile/lib
    )
endif()

if(MSVC)
    target_link_libraries(gw_lib PUBLIC ${LIBXML2_TARGET} ${LIBZIP_TARGET})
else()
    target_include_directories(gw_lib PUBLIC ${LIBXML2_INCLUDE_DIRS} ${LIBZIP_INCLUDE_DIRS})
    target_link_libraries(gw_lib PUBLIC 
        ${LIBXML2_TARGET}
        ${LIBZIP_TARGET}
        ${CMAKE_SOURCE_DIR}/external/cppuprofile/lib/libcppuprofile.so
    )
endif()

# benchmarks
add_executable(bench_divide  src/release/divide_benchmark.cpp)
target_link_libraries(bench_divide PRIVATE gw_lib)

add_executable(bench_export  src/release/export_benchmark.cpp)
target_link_libraries(bench_export PRIVATE gw_lib)

add_executable(bench_import  src/release/import_benchmark.cpp)
target_link_libraries(bench_import PRIVATE gw_lib)

add_executable(bench_merge   src/release/merge_benchmark.cpp)
target_link_libraries(bench_merge PRIVATE gw_lib)

add_executable(bench_parse_xsd src/release/parse_xsd_benchmark.cpp)
target_link_libraries(bench_parse_xsd PRIVATE gw_lib)

if(WIN32)
    add_executable(divide src/release/divide_main.cpp)
    target_link_libraries(divide PRIVATE gw_lib)
endif()

if(MINGW)
    set_target_properties(divide PROPERTIES 
        COMPILE_FLAGS "-municode"
        LINK_FLAGS "-Wl,--subsystem,console"
    )
    set_target_properties(merge PROPERTIES 
        COMPILE_FLAGS "-municode"
        LINK_FLAGS "-Wl,--subsystem,console"
    )
endif()

# DLL
file(GLOB DLL_SOURCES "dll/*.cpp")
add_library(my_dll SHARED ${DLL_SOURCES})
target_include_directories(my_dll PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(my_dll PRIVATE gw_lib)

# Definicje dla Windows
if(WIN32)
    target_compile_definitions(gw_lib PUBLIC -D_WIN32_WINNT=0x0601)
    target_compile_definitions(my_dll PRIVATE MYDLL_EXPORTS)
endif()

enable_testing()

foreach(dll_src ${DLL_SOURCES})
    get_filename_component(dll_name ${dll_src} NAME_WE)
    add_library(${dll_name} SHARED ${dll_src})
    target_include_directories(${dll_name} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
    target_link_libraries(${dll_name} PRIVATE gw_lib ${LIBXML2_LIBRARIES})

    if(WIN32)
        target_compile_definitions(${dll_name} PRIVATE MYDLL_EXPORTS)
    endif()
endforeach()

# test
