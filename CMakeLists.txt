cmake_minimum_required(VERSION 3.16)

project(gw VERSION 0.3 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -Wconversion -O3")

add_definitions(-DLIBXML_STATIC)

find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBXML2 REQUIRED libxml-2.0)
find_path(MINIZIP_INCLUDE_DIR minizip/zip.h)
find_library(MINIZIP_LIB minizip)
find_library(ZLIB_LIB z)

file(GLOB_RECURSE GW_LIBRARY_HEADERS "include/*.h")
file(GLOB_RECURSE GW_LIBRARY_SOURCES "src/*.cpp")

#gw_lib

add_library(gw_lib STATIC ${GW_LIBRARY_HEADERS} ${GW_LIBRARY_SOURCES})
target_include_directories(gw_lib PUBLIC
    ${LIBXML2_INCLUDE_DIRS}
    ${MINIZIP_INCLUDE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
target_link_libraries(gw_lib PUBLIC ${MINIZIP_LIB} ${ZLIB_LIB} ${LIBXML2_LIBRARIES})

add_executable(gw src/main.cpp)
target_link_libraries(gw PRIVATE gw_lib)

add_executable(gw_divide_exe src/release/divide.cpp)
target_link_libraries(gw_divide_exe PRIVATE gw_lib)

#my_dll

file(GLOB DLL_SOURCES "dll/*.cpp")
add_library(my_dll SHARED ${DLL_SOURCES})
target_include_directories(my_dll PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
target_link_libraries(my_dll PRIVATE gw_lib)

if (MINGW)
    message(STATUS "Building with MinGW (cross-compilation for Windows)")
    target_compile_definitions(gw_lib PUBLIC -D_WIN32_WINNT=0x0601)
    target_compile_definitions(my_dll PRIVATE MYDLL_EXPORTS)
endif()

enable_testing()

foreach(dll_src ${DLL_SOURCES})
    get_filename_component(dll_name ${dll_src} NAME_WE)

    add_library(${dll_name} SHARED ${dll_src})
    
    target_include_directories(${dll_name} PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
    )

    target_link_libraries(${dll_name} PRIVATE gw_lib ${LIBXML2_LIBRARIES})

    if(MINGW)
        target_compile_definitions(${dll_name} PRIVATE MYDLL_EXPORTS)
    endif()
endforeach()