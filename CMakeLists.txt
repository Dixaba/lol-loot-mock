project(lol_loot_mock)

set(CMAKE_CXX_STANDARD 17)

add_executable(lol_loot_mock main.cpp)
target_link_libraries(${CMAKE_PROJECT_NAME} ws2_32)