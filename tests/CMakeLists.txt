include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
    DOWNLOAD_EXTRACT_TIMESTAMP true
)
FetchContent_MakeAvailable(googletest)

add_executable(gw_tests
    basic_test.cpp
)

target_link_libraries(gw_tests PRIVATE gw_lib gtest_main)

include(GoogleTest)
gtest_discover_tests(gw_tests)