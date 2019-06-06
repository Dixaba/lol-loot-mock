cmake_minimum_required(VERSION 3.7)

project(lol_loot_mock)

set(CMAKE_CXX_STANDARD 17)

add_executable(lol_loot_mock main.cpp)

add_library(sqlite STATIC include/sqlite3.c)

IF(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    target_link_libraries(${CMAKE_PROJECT_NAME} ws2_32)
ENDIF(${CMAKE_SYSTEM_NAME} MATCHES "Windows")

IF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    target_link_libraries(${CMAKE_PROJECT_NAME} pthread)
ENDIF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")

target_link_libraries(${CMAKE_PROJECT_NAME} sqlite)