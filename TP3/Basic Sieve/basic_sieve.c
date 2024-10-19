#include "basic_sieve.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>


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
    mpz_init_set_ui(z_n, 2);

    for (int i = 0; i < k; i++) {
        primes[i] = mpz_get_ui(z_n);
        mpz_nextprime(z_n, z_n);
    }

    mpz_clear(z_n);  
    return primes;
}


// z_p is the random l-bit odd integer
void basic_sieve_prime_gen(mpz_t z_p, int b, int k, int* primes, int t)
{
    mpz_t z_remainder;
    mpz_init(z_remainder);

    while (1)
    {
        int j = 1;
        while(j <= k-1){
            mpz_mod_ui(z_remainder, z_p, primes[j]);
            if(mpz_cmp_ui(z_remainder, 0) == 0){
                mpz_add_ui(z_p, z_p, 2);
                break;
            }
            j++;
        }
        if(mpz_probab_prime_p(z_p, t) != 0){
            break;
        }
        mpz_add_ui(z_p, z_p, 2);
    }
    mpz_clear(z_remainder);
}