#ifndef __RC4_H__
#define __RC4_H__


void uc_rc4_init(unsigned int sbox, unsigned int key_addr, unsigned int Len);
void uc_rc4_crypt(unsigned int sbox, unsigned int data, unsigned int Len);
void print_hex_dump_bytes(const void *buf, size_t len);

#endif