cmake_minimum_required(VERSION 3.0.0)
project(mock VERSION 0.0.1)

add_executable(target-mock ${CMAKE_CURRENT_LIST_DIR}/target-mock.c)
target_link_libraries(target-mock PRIVATE ipc)
target_include_directories(target-mock PRIVATE ${CMAKE_CURRENT_LIST_DIR}/../ipc)
