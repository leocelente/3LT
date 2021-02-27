cmake_minimum_required(VERSION 3.0.0)
project(library_tester VERSION 0.1.0)

include(./mock/CMakeLists.txt)
include(./library/CMakeLists.txt)

add_executable(library_tester src/main.c)
target_link_libraries(library_tester PRIVATE ipc)
target_include_directories(library_tester PRIVATE ./ipc/)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
