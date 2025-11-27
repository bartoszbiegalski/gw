target_include_directories(gw_lib PUBLIC
    ${MSYS_INCLUDE}
    ${MSYS_INCLUDE}/libxml2
)

target_link_libraries(gw_lib PUBLIC
    ${MSYS_LIB}/libzip.a
    ${MSYS_LIB}/libxml2.a
    -lz
    -lbz2
    -llzma
    -lzstd
    -liconv
    -lws2_32
    -lbcrypt
   # cppuprofile
)
