cmake_minimum_required(VERSION 3.16)
project(gw VERSION 0.3 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
set(CMAKE_FIND_LIBRARY_PREFERENCES "ONLY" CACHE STRING "" FORCE)

# wspólne definicje
add_definitions(-DLIBXML_STATIC -DLIBZIP_NO_CRYPTO)

# pliki projektu
file(GLOB_RECURSE GW_LIBRARY_SOURCES
    "src/core/*.cpp"
    "src/io/*.cpp"
    "src/logic/*.cpp"
    "src/services/*.cpp"
    "src/utils/*.cpp"
)

# dll
file(GLOB DLL_SOURCES 
    "dll/gw_dll.cpp"
)

if(WIN32)
    include(cmake/windows.cmake)
elseif(UNIX)
    include(cmake/linux.cmake)
endif()

add_subdirectory(tests)

enable_testing()

add_executable(gmlWorker src/release/text_main.cpp)
add_executable(obreby src/release/obreby.cpp)

target_include_directories(obreby PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(gmlWorker PRIVATE gw_lib)
target_link_libraries(obreby PRIVATE gw_lib -static)
target_link_options(obreby PRIVATE)
