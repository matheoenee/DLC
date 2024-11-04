#ifndef STRUCT_H
#define STRUCT_H

#include <gmp.h>

typedef struct {
    mpz_t x;
    mpz_t y;
} Point;

#endif // STRUCT_H