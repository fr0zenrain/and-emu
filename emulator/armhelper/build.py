#!/usr/bin/python
import os
import sys,getopt
import shutil

CC="arm-linux-androideabi-gcc"
CXX="arm-linux-androideabi-g++"
STRIP="arm-linux-androideabi-strip"
LD="arm-linux-androideabi-ld"
STRIP="arm-linux-androideabi-strip"

MODE=''
SDK_DIR=""
INCLUDE_DIR=""
LDLIB_DIR =""
STLPORT_DIR=""
SOURCE_CXX=[]
SOURCE_ASM=[]
SOURCE_OBJ=[]
OBJECTS=[]
CFLAGS=" -c -std=c99 -MMD -MP -fpic -fno-exceptions -ffunction-sections -funwind-tables -fno-stack-protector -no-canonical-prefixes -march=armv5te -mtune=xscale -msoft-float -O2 -DNDEBUG -fomit-frame-pointer -fno-strict-aliasing -finline-limit=64  -Wa,--noexecstack -Wformat -Werror=format-security "
LDFLAGS=" --eh-frame-hdr -dynamic-linker /system/bin/linker -X -m armelf_linux_eabi -Bsymbolic --no-undefined -z noexecstack -z relro -z now "
SYSROOT=''

def strip(path):
    cmd = STRIP + ' --strip-unneeded ' + path
    os.system(cmd)
    print cmd

def getName(path):
    if path.endswith('.c'):
        return path[:-4]
    elif path.endswith('.cpp'):
        return path[:-4]
    elif path.endswith('.S'):
        return path[:-2]

def getSource(dir):
    for root, dirs, files in os.walk(dir):
        for name in files:
             path = root + '/' + name
             if path.endswith('.c'):
                SOURCE_CXX.append(path)
             elif path.endswith('.cpp'):
                SOURCE_CXX.append(path)
             elif path.endswith('.S'):
                SOURCE_ASM.append(path)
             elif path.endswith('.o'):
                SOURCE_OBJ.append(path)
    return SOURCE_CXX


def compile():
    if os.path.exists('obj') == False:
         os.mkdir('obj')
    if os.path.exists('libs') == False:
        os.mkdir('libs')
    if os.path.exists('obj/jni'):
       shutil.rmtree('obj/jni')
    os.mkdir('obj/jni')
    for file in SOURCE_CXX:
        flags = CFLAGS;
        if MODE.lower() == "debug":
            flags += ' -DDEBUG '
        cmd = CC + ' -I. -Istub -I'+INCLUDE_DIR +' -I'+STLPORT_DIR+ flags + file + ' --sysroot='+SYSROOT+ ' -o obj/'+getName(file)+'.o'
        os.system(cmd)
    for file in SOURCE_ASM:
        cmd = CC + ' -I. -Istub -I'+INCLUDE_DIR +' -I'+STLPORT_DIR+ flags + file +  ' --sysroot='+SYSROOT+' -o obj/'+getName(file)+'.o'
        os.system(cmd)

def link(dir):
    flags = LDFLAGS;
    if MODE.lower() == "release":
        flags +=  ''
    for file in SOURCE_OBJ:
        shutil.copy(file,'obj/jni')
    for root, dirs, files in os.walk(dir):
        for name in files:
             path = root + '/' + name
             if path.endswith('.o'):
                OBJECTS.append(path)
    objs = " ".join(OBJECTS)
    cmd = LD + flags + objs + ' -shared  -L'+ LDLIB_DIR +  ' --sysroot='+SYSROOT+' -ldl -lc -o '+ ' libs/libhelper.so '
    print cmd
    os.system(cmd)
    if os.path.exists('libs/libhelper.so'):
        strip('libs/libhelper.so')

if len(sys.argv) < 3:
    print 'usage: ndk path debug|release'
    sys.exit(-1)

SDK_DIR = sys.argv[1]
MODE=sys.argv[2]
INCLUDE_DIR="%s/platforms/android-8/arch-arm/usr/include" % SDK_DIR
LDLIB_DIR="%s/platforms/android-8/arch-arm/usr/lib" % SDK_DIR
SYSROOT="%s/platforms/android-8/arch-arm" % SDK_DIR
STLPORT_DIR="%s/sources/cxx-stl/stlport/stlport" % SDK_DIR
print INCLUDE_DIR
print LDLIB_DIR
print SYSROOT
getSource('jni')
compile()
link('obj/jni')

