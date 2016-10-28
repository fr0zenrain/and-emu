cmake_minimum_required(VERSION 3.3)
project(emulator)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -lunicorn")


include_directories(include)


set(SOURCEFILES
        emulator/emulator.cpp
        emulator/dlfcn.cpp
        emulator/linker.cpp
        emulator/linker_phdr.cpp
        emulator/memory.cpp
        emulator/engine.cpp
        )

add_executable(emulator ${SOURCEFILES})

