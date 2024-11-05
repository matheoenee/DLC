#include "sha256.h"

void sha256(const char *filename, unsigned char *hash) {
    // unsigned char hash[SHA256_DIGEST_LENGTH]; // Buffer for the resulting hash
    unsigned char buffer[BUFFER_SIZE];

    FILE *file = fopen(filename, "rb"); // Open the file in binary mode
    if (!file) {
        perror("Failed to open file");
        return;
    }

    // Initialize the SHA-256 context
    EVP_MD_CTX *mdctx = EVP_MD_CTX_new();
    if (!mdctx) {
        perror("Failed to create OpenSSL context");
        fclose(file);
        return;
    }
    if (EVP_DigestInit_ex(mdctx, EVP_sha256(), NULL) != 1) {
        perror("Failed to initialize digest");
        EVP_MD_CTX_free(mdctx);
        fclose(file);
        return;
    }

    // Read the file in chunks and update the hash
    size_t bytesRead;
    while ((bytesRead = fread(buffer, 1, BUFFER_SIZE, file)) > 0) {
        if (EVP_DigestUpdate(mdctx, buffer, bytesRead) != 1) {
            perror("Failed to update digest");
            EVP_MD_CTX_free(mdctx);
            fclose(file);
            return;
        }
    }

    // Finalize the hash computation
    unsigned int hashLength;
    if (EVP_DigestFinal_ex(mdctx, hash, &hashLength) != 1) {
        perror("Failed to finalize digest");
        EVP_MD_CTX_free(mdctx);
        fclose(file);
        return;
    }

    // Print the hash in hexadecimal format
    printf("SHA-256 hash of %s: ", filename);
    for (unsigned int i = 0; i < hashLength; i++) {
        printf("%02x", hash[i]);
    }
    printf("\n");

    // Cleanup
    EVP_MD_CTX_free(mdctx);
    fclose(file);
}