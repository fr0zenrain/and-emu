#ifndef CORE_COMMON_CRC32_H
#define CORE_COMMON_CRC32_H


unsigned int crc32(const void *data, unsigned int size, unsigned int oldCRC);
unsigned int getcrc32(const void *data, unsigned int size);

#endif
