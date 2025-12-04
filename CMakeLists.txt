cmake_minimum_required(VERSION 3.16)
project(gw VERSION 0.3 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# wspólne definicje
add_definitions(-DLIBXML_STATIC -DLIBZIP_NO_CRYPTO)

# FetchContent cppuprofile
# include(FetchContent)
# FetchContent_Declare(
#     cppuprofile
#     GIT_REPOSITORY https://github.com/Orange-OpenSource/cppuprofile.git
#     GIT_TAG master
# )
# set(FETCHCONTENT_UPSTREAM_MINIMUM_CMAKE_VERSION 3.16)
# FetchContent_MakeAvailable(cppuprofile)
# if(TARGET cppuprofile)
#     target_compile_options(cppuprofile PRIVATE -Wno-error)
# endif()

# pliki projektu
file(GLOB_RECURSE GW_LIBRARY_SOURCES
    "src/benchmark/benchmark.cpp"
    "src/core/*.cpp"
    #"src/external/cppuprofile/*.cpp"
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

add_executable(main  src/release/main.cpp)
target_link_libraries(main PRIVATE gw_lib)

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

add_executable(divide src/release/divide.cpp)
target_link_libraries(divide PRIVATE gw_lib)

add_executable(merge src/release/merge.cpp)
target_link_libraries(merge PRIVATE gw_lib)

add_executable(parse src/release/parse.cpp)
target_link_libraries(parse PRIVATE gw_lib)

add_executable(get_key src/release/get_key.cpp)
target_link_libraries(get_key PRIVATE gw_lib)

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
