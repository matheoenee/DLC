#ifndef SHA256_H
#define SHA256_H

#include <gmp.h>
#include <openssl/evp.h>
#include <openssl/sha.h>

#define BUFFER_SIZE 4096  // Read the file in 4096-byte chunks

// compute sha256 for a given file
void sha256(const char *filename, unsigned char *hash);

#endif //SHA256_H