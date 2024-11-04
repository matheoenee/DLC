#ifndef ADD_AND_DOUBLE_H
#define ADD_AND_DOUBLE_H

#include "struct.h"

#include <gmp.h>

// verifier si P appartient à la courbe elliptique
int on_curve(Point P, mpz_t p, mpz_t a, mpz_t b);

// additionner deux points sur la courbe
int addition(Point* R, Point P, Point Q, mpz_t p, mpz_t a, mpz_t b);

// doubler un point sur la courbe
int doubly(Point* R, Point P, mpz_t p, mpz_t a, mpz_t b);

int multiple(Point *R, Point P, mpz_t k, mpz_t p, mpz_t a, mpz_t b);

#endif //ADD_AND_DOUBLE_H