
set(SOURCEFILES
        adler32.c
        crc32.c
        deflate.c
        gzclose.c
        inflate.c
        gzlib.c
        compress.c
        infback.c
        inffast.c
        inftrees.c
        trees.c
        zutil.c
        uncompr.c
        gzwrite.c
        gzclose.c
        gzread.c
        )

add_library(zlib STATIC ${SOURCEFILES})