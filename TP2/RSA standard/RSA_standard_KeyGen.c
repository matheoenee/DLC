#include <stdio.h>
#include <stdlib.h>
#include <gmp.h>
#include <time.h>
#include <stdio.h>

void mpz_rand_k_bit_prime(mpz_t z_res, gmp_randstate_t prng, int k){
    mpz_urandomb(z_res, prng, k);
    mpz_setbit(z_res, k - 1);
    mpz_nextprime(z_res, z_res);
}

int main(int argc, char *argv[]) {
    if (argc != 3)
    {
        fprintf(stderr, "Usage : %s <bit length> <exponent>\n", argv[0]);
        return 1;
    }

    printf("RSA Key Generation (Standard mode)\n\n");

    int k = atoi(argv[1]);
    int e = atoi(argv[2]);

    // GMP variables initialization
    mpz_t z_p, z_q, z_d, z_n, z_e; // RSA standard variables
    mpz_inits(z_p, z_q, z_d, z_n, z_e, NULL);
    mpz_set_ui(z_e, e);

    mpz_t z_p1, z_q1, z_phi; // Euler's totient function
    mpz_inits(z_p1, z_q1, z_phi, NULL);

    int is_invertible = 0;

    // PRNG initialization
    gmp_randstate_t prng;
    gmp_randinit_default(prng);
    gmp_randseed_ui(prng, time(NULL));

    printf("Key generation...\n");
    while(is_invertible == 0 || mpz_sizeinbase(z_n,2) != k)
    {
        // Random generation of p
        printf("[+] generation of p...\n");
        mpz_rand_k_bit_prime(z_p, prng, k/2);
        // gmp_printf("   p = %Zu\n", z_p);
        printf("   p = 0x%s\n", mpz_get_str(NULL, 16, z_p));
        printf("   p = %s\n", mpz_get_str(NULL, 10, z_p));
        printf("   lenght : %d bits\n",mpz_sizeinbase(z_p,2));

        // Random generation of q
        printf("[+] generation of q...\n");
        mpz_rand_k_bit_prime(z_q, prng, k/2);
        // gmp_printf("   q = %Zu\n", z_q);
        printf("   q = 0x%s\n", mpz_get_str(NULL, 16, z_q));
        printf("   q = %s\n", mpz_get_str(NULL, 10, z_q));
        printf("   lenght : %d bits\n",mpz_sizeinbase(z_q,2));

        // Modulus computation
        printf("[+] modulus computation...\n");
        mpz_mul(z_n, z_p, z_q);
        // gmp_printf("   n = %Zu\n", z_n);
        printf("   n = 0x%s\n", mpz_get_str(NULL, 16, z_n));
        printf("   n = %s\n", mpz_get_str(NULL, 10, z_n));
        printf("   lenght : %d bits\n",mpz_sizeinbase(z_n,2));
        if(mpz_sizeinbase(z_n,2) != k){
            printf("   [Error] n is not %d bits long.\n",k);
            printf("   Starting new key generation...\n\n");
            continue;
        }

        // Euler's totient function computation
        mpz_sub_ui(z_p1, z_p, 1);
        mpz_sub_ui(z_q1, z_q, 1);
        mpz_mul(z_phi, z_p1, z_q1);
        printf("   phi(n) = %s\n", mpz_get_str(NULL, 10, z_phi));

        // Private exponent computation + invert test
        printf("[+] private exponent computation...\n");
        is_invertible = mpz_invert(z_d, z_e, z_phi);
        
        if(is_invertible){
            // gmp_printf("   d = %Zu\n", z_d);
            printf("   d = 0x%s\n", mpz_get_str(NULL, 16, z_d));
            printf("   d = %s\n", mpz_get_str(NULL, 10, z_d));
        }else{
            printf("   [Error] e is not invertible modulo phi(n).\n");
        }
    }
    printf("Key generation is done!\n\n");

    // Clear intermediate GMP variables
    mpz_clears(z_p1, z_q1, z_phi, NULL);

    // Export RSA variables in file
    printf("Exporting RSA keys in file...\n");
    FILE *file;
    file = fopen("RSA_standard.key", "w");
    if (file == NULL) {
        printf("   [Error] Can't open file\n");
        return 1;
    }
    fprintf(file, "e = 0x%s\n", mpz_get_str(NULL, 16, z_e));
    fprintf(file, "n = 0x%s\n", mpz_get_str(NULL, 16, z_n));
    fprintf(file, "d = 0x%s\n", mpz_get_str(NULL, 16, z_d));
    fclose(file);

    // Clear all GMP variables
    mpz_clears(z_p, z_q, z_d, z_n, z_e, NULL);

    // Clear GMP PRNG
    gmp_randclear(prng);

    printf("\nEnd of program.\n");
    return 0;
}