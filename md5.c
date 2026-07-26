#include "md5.h"

void md5_hash(const char *input, char output_hex[33]) {
    unsigned char hash[16];

    MD5((const unsigned char *)input, strlen(input), hash);

    for (int i = 0; i < 16; i++) {
        sprintf(&output_hex[i * 2], "%02x", hash[i]);
    }
}
