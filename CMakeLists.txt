cmake_minimum_required(VERSION 3.0.0)
project(prefix VERSION 0.1.0)

include(CTest)
enable_testing()


add_executable(prefix main.c)
add_subdirectory(lib)
add_subdirectory(test)
include_directories(lib)
target_link_libraries(prefix IPV4_LIB)
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
