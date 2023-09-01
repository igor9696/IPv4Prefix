# GTEST
set (CMAKE_CXX_STANDARD 20)
include(CTest)
enable_testing()
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/58d77fa8070e8cec2dc1ed015d66b454c8d78850.zip
)

FetchContent_MakeAvailable(googletest)

add_executable(UT_Prefix UT_prefix.cpp)
target_link_libraries(UT_Prefix
                    GTest::gtest_main
                    GTest::gmock_main
                    IPV4_LIB
)
include(GoogleTest)
gtest_discover_tests(UT_Prefix)

include_directories(../lib)