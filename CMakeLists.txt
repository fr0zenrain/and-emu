cmake_minimum_required(VERSION 3.3)
project(emulator)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -m32 -std=c++11 -lunicorn")# -fsanitize=address")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -m32 -DCAPSTONE_HAS_ARM -DCAPSTONE_USE_SYS_DYN_MEM")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-framework,CoreFoundation,-framework,IOKit,-framework,CoreServices,-framework,ApplicationServices")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wl,-framework,CoreFoundation ,-framework,IOKit")
#set(CMAKE_CXX_COMPILER "/usr/local/bin/clang")

include_directories(include)
include_directories(capstone)
include_directories(physfs-2.0.3)

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
        emulator/runtime/opendir.cpp
        emulator/jvm/jvm.cpp
        emulator/jvm/func.cpp
        emulator/jvm/alloc.cpp
        emulator/jvm/utfstring.cpp
        emulator/jvm/jni.cpp
        emulator/hexdump.cpp
        )

add_subdirectory(capstone)

add_executable(emulator ${SOURCEFILES})
target_link_libraries(emulator capstone)