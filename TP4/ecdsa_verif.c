#include "add_and_double.h"
#include "struct.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <openssl/evp.h>
#include <openssl/sha.h>

#define BUFFER_SIZE 4096  // Read the file in 4096-byte chunks

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


int main(int argc, char *argv[]) {

    if (argc < 2) {
        fprintf(stderr, "Usage: %s <file>\n", argv[0]);
        return 1;
    }
    
    // mpz inits
    Point G, Q, K;
    mpz_t z_p, z_a, z_b, z_n, z_d, z_m, z_k, z_r, z_s;
    mpz_inits(G.x, G.y, Q.x, Q.y, K.x, K.y, NULL);
    mpz_inits(z_p, z_a, z_b, z_n, z_d, z_m, z_k, z_r, z_s);

    // TO DO THE REST

    FILE *file = fopen("params.txt", "r");
    if (!file) {
        fprintf(stderr, "[error] : failed to open 'params.txt'.\n");
        return 1;
    }

    char line[256];
    while (fgets(line, sizeof(line), file)) {
        char *key = strtok(line, " =\n");
        char *value = strtok(NULL, " =\n");

        if (strcmp(key, "a") == 0) {
            mpz_set_str(z_a, value, 10);  // base 10 pour l'entrée
        } else if (strcmp(key, "b") == 0) {
            mpz_set_str(z_b, value, 10);
        } else if (strcmp(key, "p") == 0) {
            mpz_set_str(z_p, value, 10);
        } else if (strcmp(key, "Gx") == 0) {
            mpz_set_str(G.x, value, 10);
        } else if (strcmp(key, "Gy") == 0) {
            mpz_set_str(G.y, value, 10);
        } else if (strcmp(key, "n") == 0) {
            mpz_set_str(z_n, value, 10);
        }
    }

    fclose(file);

    file = fopen("ecdsa.key", "r");
    if (!file) {
        fprintf(stderr, "[error] : failed to open 'ecdsa.key'.\n");
        return 1;
    }

    while (fgets(line, sizeof(line), file)) {
        char *key = strtok(line, " =\n");
        char *value = strtok(NULL, " =\n");

        if (strcmp(key, "d") == 0) {
            mpz_set_str(z_d, value, 16);
        }
    }

    fclose(file);

    // test functions
    printf("TP4 - Elliptic Curve DSA (ECDSA) - Sign\n");
    printf("y² = x³ + ax + b (mod p)\n\n");

    gmp_printf("[+] a = %Zu\n", z_a);
    gmp_printf("[+] b = %Zu\n", z_b);
    gmp_printf("[+] p = %Zu\n\n", z_p);

    printf("G = (Gx, Gy)\n");

    gmp_printf("[+] Gx = %Zu\n", G.x);
    gmp_printf("[+] Gy = %Zu\n", G.y);
    gmp_printf("[+] order of G = %Zu\n\n", z_n);

    printf("Checking if G is on the curve...\n");
    if(on_curve(G, z_p, z_a, z_b)) {
        printf("[+] G is on the curve!\n\n");
    } else {
        printf("[-] G is not on the curve!\n\n");
    }

    // computing H(file) = H(m)
    unsigned char hash[SHA256_DIGEST_LENGTH];
    sha256(argv[1], hash);

    mpz_import(z_m, SHA256_DIGEST_LENGTH, 1, sizeof(hash[0]), 1, 0, hash);
    gmp_printf("[+] H(m) = %Zx\n\n", z_m);

    do
    {
        // generating random k
        gmp_randstate_t prng;
        gmp_randinit_default(prng);
        gmp_randseed_ui(prng, time(NULL));

        mpz_urandomm(z_k, prng, z_n);

        printf("Generating random k...\n");
        gmp_printf("[+] k = 0x%Zx\n\n", z_k);

        printf("Computing K = k.G ...\n");

        multiple(&K, G, z_d, z_p, z_a, z_b);

        gmp_printf("[+] Kx = 0x%Zx\n", K.x);
        gmp_printf("[+] Ky = 0x%Zx\n\n", K.y);

        printf("Checking if K is on the curve...\n");
        if(on_curve(K, z_p, z_a, z_b)) {
            printf("[+] K is on the curve!\n\n");
        } else {
            printf("[-] K is not on the curve!\n\n");
        }

        // compute r = K.x mod n
        printf("Computing r = K.x mod n...\n");
        mpz_mod(z_r, K.x, z_n);
        if(mpz_cmp_ui(z_r, 0) == 0){
            printf("[+] r = 0, let's try again...\n\n");
            continue;
        }

        // compute s = k^{-1}(H(m)+d*r)
        printf("Computing s = k^{-1}(H(m) + d.r) mod n...\n");
        mpz_invert(z_k, z_k, z_n);
        mpz_mul(z_s, z_d, z_r);
        mpz_add(z_s, z_s, z_m);
        mpz_mul(z_s, z_s, z_k);
        mpz_mod(z_s, z_s, z_n);
        if(mpz_cmp_ui(z_s, 0) == 0){
            printf("[+] s = 0, let's try again...\n\n");
            continue;
        }
    } while (mpz_cmp_ui(z_r, 0) == 0 || mpz_cmp_ui(z_s, 0) == 0);

    printf("\n[+] signature = (r,s)\n");
    gmp_printf("[+] r = %Zx\n", z_r);
    gmp_printf("[+] s = %Zx\n\n", z_s);

    // Create the .sign filename by appending ".sign" to argv[1]
    char sign_filename[256];
    snprintf(sign_filename, sizeof(sign_filename), "%s.sign", argv[1]);

    printf("[+] Writing signature into '%s'...\n", sign_filename);

    // Open the file in write mode and redirect stdout to it
    freopen(sign_filename, "w", stdout);

    gmp_printf("r=%Zx\n", z_r);
    gmp_printf("s=%Zx\n", z_s);

    fclose(stdout);

    // mpz clears
    mpz_clears(z_p, z_a, z_b, z_n, z_d, z_m, z_k, z_r, z_s, G.x, G.y, K.x, K.y, NULL);

    return 0;
}
