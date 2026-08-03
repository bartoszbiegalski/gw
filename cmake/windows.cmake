set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static -static-libgcc -static-libstdc++")
set(CMAKE_FIND_LIBRARY_SUFFIXES ".a")

set(LIBXML2_LIBRARY "C:/msys64/mingw64/lib/libxml2.a" CACHE FILEPATH "" FORCE)
set(LIBXML2_INCLUDE_DIR "C:/msys64/mingw64/include/libxml2" CACHE PATH "" FORCE)
set(LibXml2_DIR "C:/msys64/mingw64/lib/cmake/libxml2" CACHE PATH "" FORCE)
set(LibXml2_USE_STATIC_LIBS ON CACHE BOOL "" FORCE)

set(LIBZIP_LIBRARY "C:/msys64/mingw64/lib/libzip.a" CACHE FILEPATH "" FORCE)
set(LIBZIP_INCLUDE_DIR "C:/msys64/mingw64/include" CACHE PATH "" FORCE)
set(LibZip_DIR "C:/msys64/mingw64/lib/cmake/libzip" CACHE PATH "" FORCE)
set(LibZip_USE_STATIC_LIBS ON CACHE BOOL "" FORCE)

find_package(LibXml2 REQUIRED ON)
find_package(LibZip REQUIRED ON)
find_package(GEOS REQUIRED ON)

# gw_lib
add_library(gw_lib STATIC ${GW_LIBRARY_SOURCES})
target_compile_definitions(gw_lib PUBLIC ZIP_STATIC)
target_compile_definitions(gw_lib PUBLIC LIBXML_STATIC)

target_include_directories(gw_lib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_BINARY_DIR}
    ${LIBZIP_INCLUDE_DIR}
)

target_link_libraries(gw_lib PUBLIC
    LibXml2::LibXml2
    ${LIBZIP_LIBRARY}
    GEOS::geos
    -lz
    -lbz2
    -llzma
    -lzstd
    -liconv
    -lws2_32
    -lbcrypt
)


# gw_dll
add_library(gw_dll SHARED ${DLL_SOURCES})
target_link_options(gw_dll PRIVATE
    -static
    -static-libstdc++
    -static-libgcc
)
target_include_directories(gw_dll PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/include

)
target_link_libraries(gw_dll PRIVATE
    gw_lib
)
