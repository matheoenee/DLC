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
    Point G, K;
    mpz_t z_p, z_a, z_b, z_n, z_d, z_m, z_k, z_r, z_s;
    mpz_inits(z_p, z_a, z_b, z_n, z_d, z_m, z_k, z_r, z_s, G.x, G.y, K.x, K.y, NULL);

    printf("TP4 - Elliptic Curve DSA (ECDSA) - Sign\n");
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

    printf("Reading private key from file...\n\n");
    read_private_key("ecdsa.key", &z_d);

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

        multiple(&K, G, z_k, z_p, z_a, z_b);

        gmp_printf("[+] Kx = 0x%Zx\n", K.x);
        gmp_printf("[+] Ky = 0x%Zx\n\n", K.y);

        printf("Checking if K is on the curve...\n");
        if(on_curve(K, z_p, z_a, z_b)) {
            printf("[+] K is on the curve!\n\n");
        } else {
            printf("[-] K is not on the curve!\n\n");
        }

        // compute r = K.x mod n
        printf("Computing r = K.x (mod n)...\n");
        mpz_mod(z_r, K.x, z_n);
        if(mpz_cmp_ui(z_r, 0) == 0){
            printf("[+] r = 0, let's try again...\n\n");
            continue;
        }

        // compute s = k^{-1}(H(m)+d*r)
        printf("Computing s = k^{-1}(H(m) + d.r) (mod n)...\n");
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
