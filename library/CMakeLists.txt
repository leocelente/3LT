cmake_minimum_required(VERSION 3.0.0)
project(library VERSION 0.0.1)

include(${CMAKE_CURRENT_LIST_DIR}/../ipc/CMakeLists.txt)

add_library(target ${CMAKE_CURRENT_LIST_DIR}/target/bsp.h ${CMAKE_CURRENT_LIST_DIR}/target/target.h ${CMAKE_CURRENT_LIST_DIR}/target/target.c)
target_include_directories(target PRIVATE ${CMAKE_CURRENT_LIST_DIR})

target_link_libraries(target PUBLIC ipc)
target_include_directories(target PRIVATE ${CMAKE_CURRENT_LIST_DIR}/../ipc)

add_executable(library-tests ${CMAKE_CURRENT_LIST_DIR}/tests/target-test.c)
target_link_libraries(library-tests PRIVATE target)
target_include_directories(library-tests PRIVATE ${CMAKE_CURRENT_LIST_DIR}/)

target_link_libraries(library-tests PUBLIC ipc)
target_include_directories(library-tests PRIVATE ${CMAKE_CURRENT_LIST_DIR}/../ipc)

