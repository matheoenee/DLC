#include "add_and_double.h"
#include "read_data.h"
#include "sha256.h"
#include "struct.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int main(int argc, char *argv[]) {

    if (argc < 2) {
        fprintf(stderr, "Usage: %s <file>\n", argv[0]);
        return 1;
    }
    
    // mpz inits
    Point G, Q, K;
    mpz_t z_p, z_a, z_b, z_n, z_d, z_m, z_k, z_r, z_s, z_u1, z_u2;
    mpz_inits(G.x, G.y, Q.x, Q.y, K.x, K.y, NULL);
    mpz_inits(z_p, z_a, z_b, z_n, z_d, z_m, z_k, z_r, z_s, z_u1, z_u2, NULL);

    printf("TP4 - Elliptic Curve DSA (ECDSA) - Verify\n");
    printf("y² = x³ + ax + b (mod p)\n\n");


    printf("Reading Elliptic curve parameters from file...\n");
    read_parameters("params.txt", &z_a, &z_b, &z_p, &G.x, &G.y, &z_n);

    gmp_printf("[+] a = %Zu\n", z_a);
    gmp_printf("[+] b = %Zu\n", z_b);
    gmp_printf("[+] p = %Zu\n\n", z_p);

    printf("G = (Gx, Gy)\n");

    gmp_printf("[+] Gx = %Zu\n", G.x);
    gmp_printf("[+] Gy = %Zu\n", G.y);
    gmp_printf("[+] ord(G) = n = %Zu\n", z_n);

    size_t size_n = mpz_sizeinbase(z_n, 2);
    printf("[+] size of n = %zu bits\n\n", size_n);

    printf("Checking if the order of G is prime...\n");
    if(mpz_probab_prime_p(z_n, 5) != 0) {
        printf("[+] n is prime.\n\n");
    }else {
        printf("[+] n is not prime!\n\n");
    }

    printf("Checking if G is on the curve...\n");
    if(on_curve(G, z_p, z_a, z_b)) {
        printf("[+] G is on the curve!\n\n");
    } else {
        printf("[-] G is not on the curve!\n\n");
    }

    printf("Reading public key from file...\n");
    read_public_key("ecdsa.key", &Q.x, &Q.y);

    gmp_printf("[+] Qx = %Zu\n", Q.x);
    gmp_printf("[+] Qy = %Zu\n\n", Q.y);

    // get the .sign filename by appending ".sign" to argv[1]
    char sign_filename[256];
    snprintf(sign_filename, sizeof(sign_filename), "%s.sign", argv[1]);

    printf("Reading signature from file...\n");
    read_signature(sign_filename, &z_r, &z_s);

    printf("[+] signature = (r,s)\n");
    gmp_printf("[+] r = %Zx\n", z_r);
    gmp_printf("[+] s = %Zx\n\n", z_s);

    // computing H(file) = H(m)
    printf("Computing SHA256(file)...\n");
    unsigned char hash[SHA256_DIGEST_LENGTH];
    sha256(argv[1], hash);
    mpz_import(z_m, SHA256_DIGEST_LENGTH, 1, sizeof(hash[0]), 1, 0, hash);
    gmp_printf("[+] H(m) = %Zx\n", z_m);

    size_t size_m = mpz_sizeinbase(z_m, 2);
    printf("[+] size of H(m) before truncate = %zu bits\n", size_m);

    mpz_fdiv_r_2exp(z_m, z_m, size_n); // keep only the lower size_n bits
    size_m = mpz_sizeinbase(z_m, 2);
    printf("[+] size of H(m) after truncate = %zu bits\n\n", size_m);

    // computing u1 = s^{-1}*H(m) mod n
    mpz_invert(z_u1, z_s, z_n);
    mpz_mul(z_u1, z_u1, z_m);
    mpz_mod(z_u1, z_u1, z_n);

    // computing u2 = s^{-1}*r mod n
    mpz_invert(z_u2, z_s, z_n);
    mpz_mul(z_u2, z_u2, z_r);
    mpz_mod(z_u2, z_u2, z_n);

    // computing K = u1.G + u2.Q
    printf("Computing K = u1.G + u2.Q...\n");
    multiple(&G, G, z_u1, z_p, z_a, z_b);
    multiple(&Q, Q, z_u2, z_p, z_a, z_b);
    addition(&K, G, Q, z_p, z_a, z_b);
    gmp_printf("[+] Kx = %Zx\n", K.x);
    gmp_printf("[+] Ky = %Zx\n\n", K.y);

    // verifying
    if(mpz_cmp(z_r, K.x) == 0){
        printf("[+] signature is valid!\n");
    }else{
        printf("[+] signature is not valid!\n");
    }

    // mpz clears
    mpz_clears(z_p, z_a, z_b, z_n, z_d, z_m, z_k, z_r, z_s, z_u1, z_u2, NULL);
    mpz_clears(G.x, G.y, Q.x, Q.y, K.x, K.y, NULL);

    return 0;
}
