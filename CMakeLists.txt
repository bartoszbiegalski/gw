cmake_minimum_required(VERSION 3.16)
project(gw VERSION 0.3 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

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

add_library(gw_lib STATIC ${GW_LIBRARY_SOURCES})
target_compile_definitions(gw_lib PUBLIC ZIP_STATIC)
target_compile_definitions(gw_lib PUBLIC LIBXML_STATIC)

target_include_directories(gw_lib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_BINARY_DIR}
)

add_subdirectory(tests)

add_executable(gmlWorker src/release/text_main.cpp)
target_link_libraries(gmlWorker PRIVATE gw_lib)


# file(GLOB DLL_SOURCES "dll/*.cpp")
# add_library(my_dll SHARED ${DLL_SOURCES})
# target_include_directories(my_dll PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
# target_link_libraries(my_dll PRIVATE gw_lib)

enable_testing()

if(WIN32)
    include(cmake/windows.cmake)
elseif(UNIX)
    include(cmake/linux.cmake)
endif()
