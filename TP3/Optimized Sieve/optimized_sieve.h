#ifndef OPTIMIZED_SIEVE_H
#define OPTIMIZED_SIEVE_H

#include <gmp.h>

// check residues
int exist_null_residue(int *res, int k);

// get k first prime number
int* get_k_first_prime(int k);

// z_p is the random l-bit odd integer
void optimized_sieve_prime_gen(mpz_t z_p, int b, int k, int* primes, int t);

#endif //OPTIMIZED_SIEVE_H