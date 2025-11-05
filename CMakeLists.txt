cmake_minimum_required(VERSION 3.16)
project(gw VERSION 0.3 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -Wconversion -O3")

add_definitions(-DLIBXML_STATIC)
add_definitions(-DLIBZIP_NO_CRYPTO)

# -------------------------------
# libxml2
# -------------------------------
if(MINGW)
    # Cross-kompilacja Windows z Linuxa (MXE)
    set(LIBXML2_INCLUDE_DIR "/home/bartek/mxe/usr/x86_64-w64-mingw32.static/include/libxml2")
    set(LIBXML2_LIBRARIES "/home/bartek/mxe/usr/x86_64-w64-mingw32.static/lib/libxml2.a")
    set(ZLIB_LIB "/home/bartek/mxe/usr/x86_64-w64-mingw32.static/lib/libz.a")

    set(BZIP2_LIB "/home/bartek/mxe/usr/x86_64-w64-mingw32.static/lib/libbz2.a")
    set(ICONV_LIB "/home/bartek/mxe/usr/x86_64-w64-mingw32.static/lib/libiconv.a")
    set(BCRYPT_LIB "bcrypt")

elseif(WIN32)
    # Windows natywnie (Visual Studio)
    set(LIBXML2_INCLUDE_DIR "C:/libxml-2.0/include")
    set(LIBXML2_LIBRARIES "C:/Libraries/libxml2/lib/libxml2.lib")
else()
    # Linux
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(LIBXML2 REQUIRED libxml-2.0)
    set(LIBXML2_INCLUDE_DIR ${LIBXML2_INCLUDE_DIRS})
    set(LIBXML2_LIBRARIES ${LIBXML2_LIBRARIES})
endif()

# -------------------------------
# libzip i zlib (zamiast minizip)
# -------------------------------
if(MINGW)
    # Cross-kompilacja Windows z Linuxa (MXE)
    set(LIBZIP_INCLUDE_DIR "/home/bartek/mxe/usr/x86_64-w64-mingw32.static/include/libzip")
    set(LIBZIP_LIB "/home/bartek/mxe/usr/x86_64-w64-mingw32.static/lib/libzip.a")
    set(ZLIB_LIB "/home/bartek/mxe/usr/x86_64-w64-mingw32.static/lib/libz.a")
    set(ICONV_LIB "/home/bartek/mxe/usr/x86_64-w64-mingw32.static/lib/libiconv.a")
    set(BCRYPT_LIB "bcrypt")
elseif(WIN32)
    set(LIBZIP_INCLUDE_DIR "C:/Libraries/libzip/include")
    set(LIBZIP_LIB "C:/Libraries/libzip/lib/libzip.lib")
    set(ZLIB_LIB "C:/Libraries/zlib/lib/zlib.lib")
else()
    # Linux
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(LIBZIP REQUIRED libzip)
    set(LIBZIP_INCLUDE_DIR ${LIBZIP_INCLUDE_DIRS})
    set(LIBZIP_LIB ${LIBZIP_LIBRARIES})
    set(ZLIB_LIB z)
endif()


# -------------------------------
# Pliki projektu
# -------------------------------
file(GLOB_RECURSE GW_LIBRARY_HEADERS "include/*.h")
file(GLOB_RECURSE GW_LIBRARY_SOURCES
    "src/core/*.cpp"
    "src/io/*.cpp"
    "src/logic/*.cpp"
    "src/services/*.cpp"
    "src/utils/*.cpp"
)

# -------------------------------
# Biblioteka statyczna gw_lib
# -------------------------------
add_library(gw_lib STATIC ${GW_LIBRARY_HEADERS} ${GW_LIBRARY_SOURCES})
target_include_directories(gw_lib PUBLIC
    ${LIBXML2_INCLUDE_DIR}
    ${LIBZIP_INCLUDE_DIR}         
    ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
target_link_libraries(gw_lib PUBLIC
    ${LIBZIP_LIB}
    ${ZLIB_LIB}
    ${LIBXML2_LIBRARIES}
)

if(MINGW)
    target_link_libraries(gw_lib PUBLIC
        ${LIBZIP_LIB}
        ${LIBXML2_LIBRARIES}  # najpierw libxml2
        ${ZLIB_LIB}            # potem zlib
        ${BZIP2_LIB}
        ${ICONV_LIB}
        ${BCRYPT_LIB}
    )
else()
    target_link_libraries(gw_lib PUBLIC
        ${LIBZIP_LIB}
        ${ZLIB_LIB}
        ${LIBXML2_LIBRARIES}
    )
endif()

# -------------------------------
# Wykonywalne
# -------------------------------
add_executable(gw src/release/main.cpp)
target_link_libraries(gw PRIVATE gw_lib)

if(WIN32)
    add_executable(divide src/release/divide_main.cpp)
    target_link_libraries(divide PRIVATE gw_lib)
endif()

if(MINGW)
    set_target_properties(divide PROPERTIES 
        COMPILE_FLAGS "-municode"
        LINK_FLAGS "-Wl,--subsystem,console"
    )
endif()


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
