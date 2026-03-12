cmake_minimum_required(VERSION 3.10)
project(1proj)

set(CMAKE_CXX_STANDARD 17)

include_directories(include)

add_executable(main
        src/main.cpp
        include/DataLoader.h
        include/Graph.h
        include/MutablePriorityQueue.h
        src/DataLoader.cpp
        src/Menu.cpp
        include/Menu.h
)

