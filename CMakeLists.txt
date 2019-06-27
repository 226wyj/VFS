cmake_minimum_required(VERSION 3.14)
project(MyFileSystem)

set(CMAKE_CXX_STANDARD 14)

add_executable(MyFileSystem main.cpp FileSystem.cpp FileSystem.h BlockManager.cpp BlockManager.h FileManager.cpp FileManager.h)