#include "optimized_sieve.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int exist_null_residue(int* res, int k){
    for(int i=0; i<k; i++){
        if(res[i] == 0){
            return 1;
        }
    }
    return 0;
}

// except 2
int* get_k_first_prime(int k) {
    if (k <= 0) {
        return NULL;
    }

    int* primes = (int*) malloc(k * sizeof(int));
    if (!primes) {
        printf("[Error] memory allocation failed.\n");
        return NULL;
    }

    mpz_t z_n;  
    mpz_init_set_ui(z_n, 3);

    for (int i = 0; i < k; i++) {
        primes[i] = mpz_get_ui(z_n);
        mpz_nextprime(z_n, z_n);
    }

    mpz_clear(z_n);  
    return primes;
}


// z_p is the random l-bit odd integer
void optimized_sieve_prime_gen(mpz_t z_p, int b, int k, int* primes, int t)
{
    mpz_t z_remainder;
    mpz_init(z_remainder);

    int* residues = (int*) malloc(k * sizeof(int));
    if (!residues) {
        printf("[Error] memory allocation failed.\n");
        exit;
    }

    // initialization of residues
    for(int i=0; i<k; i++){
        mpz_mod_ui(z_remainder, z_p, primes[i]);
        residues[i] = mpz_get_ui(z_remainder);
    }
    mpz_clear(z_remainder);

    while (1)
    {
        // update residues
        while (exist_null_residue(residues, k))
        {
            for(int i=0; i<k; i++){
                residues[i] = (residues[i] + 2) % primes[i];
            }
            mpz_add_ui(z_p, z_p, 2);
        }

        if(mpz_probab_prime_p(z_p, t) != 0){
            break; // exit infinity loop
        }

        // update residues
        for(int i=0; i<k; i++){
            residues[i] = (residues[i] + 2) % primes[i];
        }
        mpz_add_ui(z_p, z_p, 2);
    }


}