#ifndef READ_DATA_H
#define READ_DATA_H

#include <gmp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Read elliptic curve parameters from a given file (a,b,p,Gx,Gy,n)
void read_parameters(const char *filename, mpz_t *a, mpz_t *b, mpz_t *p, mpz_t *Gx, mpz_t *Gy, mpz_t *n);

// Read private key from a given file (d)
void read_private_key(const char *filename, mpz_t *d);

// Read public key from a given file (Qx,Qy)
void read_public_key(const char *filename, mpz_t *Qx, mpz_t *Qy);

// Read signature from a given file (r,s)
void read_signature(const char *filename, mpz_t *r, mpz_t *s);

#endif //READ_DATA_H