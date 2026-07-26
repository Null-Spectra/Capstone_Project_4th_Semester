#include "md5.h"

/* Ultra-simple mock MD5 function: generates a 32-character hash string */
void md5_hash(const char *input, char output_hex[33]) {
    unsigned int sum = 0;
    int len = strlen(input);

    // Sum up character ASCII values
    for (int i = 0; i < len; i++) {
        sum += (unsigned char)input[i] * (i + 1);
    }

    // Format into 4 8-digit numbers to total exactly 32 characters
    snprintf(output_hex, 33, "%08u%08u%08u%08u",
             sum % 100000000,
             (sum + 1234567) % 100000000,
             (sum + 7654321) % 100000000,
             (sum + 9999999) % 100000000);
}
