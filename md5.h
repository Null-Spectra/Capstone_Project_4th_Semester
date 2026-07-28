#ifndef MD5_H
#define MD5_H

#include <stdio.h>
#include <string.h>

void simple_hex_hash(const char text[], char output[33]);
void md5_hash(const char *input, char output_hex[33]);

#endif /* MD5_H */
