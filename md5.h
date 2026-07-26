#ifndef MD5_H
#define MD5_H

#define OPENSSL_SUPPRESS_DEPRECATED
#include <stdio.h>
#include <string.h>
#include <openssl/md5.h>

void md5_hash(const char *input, char output_hex[33]);

#endif /* MD5_H */
