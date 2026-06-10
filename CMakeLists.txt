cmake_minimum_required(VERSION 3.15)
project(MissileCommander)

set(CMAKE_CXX_STANDARD 17)

find_package(raylib REQUIRED)

# add_executable(MissileCommander missile_commander.cpp)
add_executable(MissileCommander main.cpp Game.cpp)

target_link_libraries(MissileCommander raylib)

target_compile_options(MissileCommander PRIVATE -Wno-narrowing)