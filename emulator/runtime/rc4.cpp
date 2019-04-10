#include <stdio.h>
#include <string.h>
#include "emulator.h"
#include "rc4.h"


extern uc_engine* g_uc;

void uc_rc4_init(unsigned int sbox, unsigned int key_addr, unsigned int Len)
{
    int i = 0;
    int j = 0;
    int plen = 0;
    unsigned char* kbuf = (unsigned char*)malloc(Len);
    if (kbuf == NULL){
        return;
    }
    uc_mem_read(g_uc, key_addr, kbuf, Len);
    printf("rc4 key:\n");
    plen = Len > 32 ? 32 : Len;
    print_hex_dump_bytes(kbuf, plen);
    unsigned char k[256] = {0};
    unsigned char s[256] = {0};
    unsigned char tmp = 0;
    for (i = 0; i < 256; i++) {
        s[i] = i;
        k[i] = kbuf[i % Len];
    }
    for (i = 0; i < 256; i++) {
        j = (j + s[i] + k[i]) % 256;
        tmp = s[i];
        s[i] = s[j]; //交换s[i]和s[j]
        s[j] = tmp;
    }
    free(kbuf);
    uc_mem_write(g_uc, sbox, s, 256);
}
void uc_rc4_crypt(unsigned int sbox, unsigned int Data, unsigned int Len) //加解密
{
    int i = 0;
    int j = 0;
    int t = 0;
    unsigned long k = 0;
    unsigned char s[256] = {0};
    unsigned char* dbuf = (unsigned char*)malloc(Len);
    if (dbuf == NULL){
        return;
    }
    uc_mem_read(g_uc,sbox,s,256);
    uc_mem_read(g_uc,Data,dbuf,Len);
    unsigned char tmp = 0;
    for (k = 0; k < Len; k++) {
        i = (i + 1) % 256;
        j = (j + s[i]) % 256;
        tmp = s[i];
        s[i] = s[j]; //交换s[x]和s[y]
        s[j] = tmp;
        t = (s[i] + s[j]) % 256;
        dbuf[k] ^= s[t];
    }
    free(dbuf);
    printf("rc4 decrypt len=0x%x\n",Len);
    uc_mem_write(g_uc, Data, dbuf, Len);
}
