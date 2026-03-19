find_package(LibXml2 REQUIRED ON)
find_package(LIBZIP REQUIRED libzip)

# gw_lib
add_library(gw_lib STATIC ${GW_LIBRARY_SOURCES})
target_compile_definitions(gw_lib PUBLIC ZIP_STATIC)
target_compile_definitions(gw_lib PUBLIC LIBXML_STATIC)

target_include_directories(gw_lib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_BINARY_DIR}
)

target_link_libraries(gw_lib PUBLIC
    LibXml2::LibXml2
    libzip::zip
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
    iconv
    xml2
    zip
)

