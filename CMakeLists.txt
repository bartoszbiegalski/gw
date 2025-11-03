cmake_minimum_required(VERSION 3.16)

project(gw VERSION 0.3 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -Wconversion -O3")

add_definitions(-DLIBXML_STATIC)

# -------------------------------
# libxml2
# -------------------------------
if(WIN32)
    # Windows: podaj ścieżki do libxml2 ręcznie lub użyj vcpkg
    set(LIBXML2_INCLUDE_DIR "C:/Libraries/libxml2/include")
    set(LIBXML2_LIBRARIES "C:/Libraries/libxml2/lib/libxml2.lib")
else()
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(LIBXML2 REQUIRED libxml-2.0)

    set(LIBXML2_INCLUDE_DIR ${LIBXML2_INCLUDE_DIRS})
    set(LIBXML2_LIBRARIES ${LIBXML2_LIBRARIES})
endif()
# -------------------------------
# minizip i zlib
# -------------------------------
if(WIN32)
    # Windows: podaj ścieżki ręcznie, jeśli masz minizip/zlib w zewnętrznym folderze
    set(MINIZIP_INCLUDE_DIR "C:/Libraries/minizip/include")
    set(MINIZIP_LIB "C:/Libraries/minizip/lib/minizip.lib")
    set(ZLIB_LIB "C:/Libraries/zlib/lib/zlib.lib")
else()
    find_path(MINIZIP_INCLUDE_DIR minizip/zip.h)
    find_library(MINIZIP_LIB minizip)
    find_library(ZLIB_LIB z)
endif()

# -------------------------------
# Pliki projektu
# -------------------------------
file(GLOB_RECURSE GW_LIBRARY_HEADERS "include/*.h")
file(GLOB_RECURSE GW_LIBRARY_SOURCES "src/*.cpp")

# -------------------------------
# Biblioteka statyczna gw_lib
# -------------------------------
add_library(gw_lib STATIC ${GW_LIBRARY_HEADERS} ${GW_LIBRARY_SOURCES})
target_include_directories(gw_lib PUBLIC
    ${LIBXML2_INCLUDE_DIR}
    ${MINIZIP_INCLUDE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
target_link_libraries(gw_lib PUBLIC ${MINIZIP_LIB} ${ZLIB_LIB} ${LIBXML2_LIBRARIES})

# -------------------------------
# Wykonywalne
# -------------------------------
add_executable(gw src/main.cpp)
target_link_libraries(gw PRIVATE gw_lib)

add_executable(gw_divide_exe src/release/divide.cpp)
target_link_libraries(gw_divide_exe PRIVATE gw_lib)

# -------------------------------
# DLL
# -------------------------------
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
