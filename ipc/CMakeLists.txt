cmake_minimum_required(VERSION 3.0.0)
project(ipc VERSION 0.0.1)

add_library(ipc ${CMAKE_CURRENT_LIST_DIR}/ipc.h ${CMAKE_CURRENT_LIST_DIR}/ipc.c)
target_include_directories(ipc PRIVATE ${CMAKE_CURRENT_LIST_DIR})
