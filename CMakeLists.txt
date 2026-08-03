cmake_minimum_required(VERSION 3.30)

set(CMAKE_TOOLCHAIN_FILE "${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake"
    CACHE STRING "vcpkg toolchain file"
)

project(gw VERSION 2.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
# set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
# set(CMAKE_FIND_LIBRARY_PREFERENCES "ONLY" CACHE STRING "" FORCE)

# Static linking
# add_definitions(-DLIBXML_STATIC -DLIBZIP_NO_CRYPTO)

# Project files
file(GLOB_RECURSE GW_LIBRARY_SOURCES
    "src/core/*.cpp"
    "src/io/*.cpp"
    "src/logic/*.cpp"
    "src/services/*.cpp"
    "src/utils/*.cpp"
)

# dll
file(GLOB DLL_SOURCES 
    #"dll/gw_dll.cpp"
)

if(WIN32)
    include(cmake/windows.cmake)
elseif(UNIX)
    include(cmake/linux.cmake)
endif()

add_subdirectory(tests)
enable_testing()

#add_executable(gmlWorker src/release/text_main.cpp)
#add_executable(obreby src/release/obreby.cpp)

# target_include_directories(obreby PRIVATE
#     ${CMAKE_CURRENT_SOURCE_DIR}/src
# )

#target_link_libraries(gmlWorker PRIVATE gw_lib)
#target_link_libraries(obreby PRIVATE gw_lib -static)
#target_link_options(obreby PRIVATE)
