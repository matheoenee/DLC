#ifndef BASIC_SIEVE_H
#define BASIC_SIEVE_H

#include <gmp.h>

// get k first prime number
int* get_k_first_prime(int k);

// z_p is the random l-bit odd integer
void basic_sieve_prime_gen(mpz_t z_p, int b, int k, int* primes, int t);

#endif //BASIC_SIEVE_H