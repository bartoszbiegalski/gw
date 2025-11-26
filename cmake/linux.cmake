add_compile_options(-Wall -Wextra -Wpedantic -Wconversion -O3)

find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBXML2 REQUIRED libxml-2.0)
pkg_check_modules(LIBZIP REQUIRED libzip)

target_include_directories(gw_lib PUBLIC
    ${LIBXML2_INCLUDE_DIRS}
    ${LIBZIP_INCLUDE_DIRS}
)

target_link_libraries(gw_lib PUBLIC
    ${LIBXML2_LIBRARIES}
    ${LIBZIP_LIBRARIES}
    #cppuprofile
)
