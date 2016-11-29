cmake_minimum_required(VERSION 3.3)
project(emulator)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -m32 -std=c++11 -lunicorn")# -fsanitize=address")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -m32 -DCAPSTONE_HAS_ARM -DCAPSTONE_USE_SYS_DYN_MEM")
#set(CMAKE_CXX_COMPILER "/usr/local/bin/clang")

include_directories(include)
include_directories(capstone)


set(SOURCEFILES
        emulator/main.cpp
        emulator/dlfcn.cpp
        emulator/linker.cpp
        emulator/linker_phdr.cpp
        emulator/memory.cpp
        emulator/engine.cpp
        emulator/crc32.cpp
        emulator/runtime/libc.cpp
        emulator/runtime/emulator.cpp
        emulator/runtime/malloc.cpp
        emulator/runtime/system.cpp
        emulator/jvm/jvm.cpp
        )

add_subdirectory(capstone)

add_executable(emulator ${SOURCEFILES})
target_link_libraries(emulator capstone)

