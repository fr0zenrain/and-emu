cmake_minimum_required(VERSION 3.3)
project(emulator)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -m32 -std=c++11 -lunicorn")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -m32 -DCAPSTONE_HAS_ARM -DCAPSTONE_USE_SYS_DYN_MEM")


include_directories(include)


set(SOURCEFILES
        emulator/emulator.cpp
        emulator/dlfcn.cpp
        emulator/linker.cpp
        emulator/linker_phdr.cpp
        emulator/memory.cpp
        emulator/engine.cpp
        emulator/crc32.cpp
        emulator/runtime/libc.cpp

        )

add_subdirectory(capstone)

add_executable(emulator ${SOURCEFILES})
target_link_libraries(emulator capstone)

