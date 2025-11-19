if(MSVC)
    add_compile_options(/W4 /permissive-)
    target_link_libraries(gw_lib PUBLIC ${LIBXML2_INCLUDE_DIRS} ${LIBZIP_INCLUDE_DIRS})
endif()

find_package(LibXml2 CONFIG REQUIRED)
find_package(libzip CONFIG REQUIRED)

if(TARGET LibXml2::LibXml2)
    target_link_libraries(gw_lib PUBLIC LibXml2::LibXml2 libzip::libzip cppuprofile)
else()
    target_link_libraries(gw_lib PUBLIC ${LIBXML2_LIBRARIES} ${LIBZIP_LIBRARIES} cppuprofile)
endif()

target_compile_definitions(gw_lib PUBLIC -D_WIN32_WINNT=0x0601)
target_compile_definitions(my_dll PRIVATE MYDLL_EXPORTS)
