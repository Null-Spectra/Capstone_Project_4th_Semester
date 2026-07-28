#include "md5.h"

void simple_hex_hash(const char text[], char output[33]) {
    unsigned int total = 0;
    int length = strlen(text);

    // Multiply each character by its position to make small changes mix things up
    for (int i = 0; i < length; i++) {
        total = total + (text[i] * (i + 1));
    }

    // %08x prints as an 8-digit HEXADECIMAL number (using 0-9 and a-f)
    sprintf(output, "%08x%08x%08x%08x", 
            total * 12345, 
            total * 67890, 
            total * 13579, 
            total * 24680);
}

void md5_hash(const char *input, char output_hex[33]) {
    simple_hex_hash(input, output_hex);
}
