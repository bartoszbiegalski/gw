cmake_minimum_required(VERSION 3.16)
project(gw VERSION 0.3 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

<<<<<<< Updated upstream
# wspólne definicje
add_definitions(-DLIBXML_STATIC -DLIBZIP_NO_CRYPTO)
=======
if(Win32)
    set(LIBXML2_INCLUDE_DIR "C:/libxml-2.0/include")
    set(LIBXML2_LIBRARIES "C:/libxml-2.0/lib/libxml2.lib")
else()

endif()

add_definitions(-DLIBXML_STATIC)
>>>>>>> Stashed changes

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
target_link_libraries(gmlWorker PRIVATE gw_lib)



