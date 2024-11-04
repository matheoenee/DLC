#include "add_and_double.h"
#include "struct.h"

#include <stdio.h>

int on_curve(Point P, mpz_t p, mpz_t a, mpz_t b) {
    mpz_t left, right, a_x;
    mpz_inits(left, right, a_x, NULL);

    // compute left part (y²)
    mpz_powm_ui(left, P.y, 2, p);

    // compute right part (x³ + ax + b)
    mpz_powm_ui(right, P.x, 3, p);
    mpz_mul(a_x, P.x, a);
    mpz_add(right, right, a_x);
    mpz_add(right, right, b);
    mpz_mod(right, right, p);

    int cmp = mpz_cmp(right, left);

    // clears
    mpz_clear(left);
    mpz_clear(right);
    mpz_clear(a_x);

    // compare
    if(cmp != 0) {
        return 0;
    } else {
        return 1;
    }
}

int is_infinity(Point P) {
    return (mpz_cmp_ui(P.x, 0) == 0 && mpz_cmp_ui(P.y, 0)== 0);
}

int addition(Point *R, Point P, Point Q, mpz_t p, mpz_t a, mpz_t b) {
    
    // checking if P or Q is the point at the infinity
    if(is_infinity(P)){
        mpz_set(R->x, Q.x);
        mpz_set(R->y, Q.y);
        return 0;
    } else if(is_infinity(Q)) {
        mpz_set(R->x, P.x);
        mpz_set(R->y, P.y);
        return 0;
    }
    
    // checking if P and Q are on the curve
    if(!on_curve(P, p, a, b) || !on_curve(Q, p, a, b)) {
        return -1;
    }

    mpz_t lambda, Rx, Ry, subX, subY;
    mpz_inits(lambda, Rx, Ry, subX, subY, NULL);
    
    mpz_sub(subX, Q.x, P.x);
    mpz_sub(subY, Q.y, P.y);

    // check if subX is zero (P and Q have the same x-coordinate)
    if (mpz_sgn(subX) == 0) {
        if (mpz_sgn(subY) == 0) {
            // clears
            mpz_clears(lambda, Rx, Ry, subX, subY, NULL);
            return doubly(R, P, p, a, b);
        } else {
            // P and Q are the same x-coordinate but different y (vertical line)
            // This should return the point at infinity (or a special value)
            mpz_set_ui(R->x, 0);
            mpz_set_ui(R->y, 0);
        }
    } else {
        // compute lambda = (Q.y - P.y) / (Q.x - P.x)
        mpz_invert(subX, subX, p);
        mpz_mul(lambda, subY, subX);
        mpz_mod(lambda, lambda, p);

        // compute Rx = lambda² - Px - Qx
        mpz_powm_ui(Rx, lambda, 2, p);
        mpz_sub(Rx, Rx, P.x);
        mpz_sub(Rx, Rx, Q.x);
    
        // compute Ry = lambda*(Px - Rx) - Py
        mpz_sub(subX, P.x, Rx);
        mpz_mul(Ry, lambda, subX);
        mpz_sub(Ry, Ry, P.y);

        // assing values
        mpz_mod(R->x, Rx, p);
        mpz_mod(R->y, Ry, p);
    }
    
    // clears
    mpz_clears(lambda, Rx, Ry, subX, subY, NULL);
    
    return 0;
}

int doubly(Point *R, Point P, mpz_t p, mpz_t a, mpz_t b) {
    // checking if P is the point at the infinity
    if(is_infinity(P)){
        mpz_set_ui(R->x, 0);
        mpz_set_ui(R->y, 0);
        return 0;
    }

    // checking if P is on the curve
    if(!on_curve(P, p, a, b)){
        return -1;
    }

    mpz_t lambda, Rx, Ry, value;
    mpz_inits(lambda, Rx, Ry, value, NULL);

    // compute lambda
    mpz_powm_ui(lambda, P.x, 2, p);
    mpz_mul_ui(lambda, lambda, 3);
    mpz_add(lambda, lambda, a);

    mpz_mul_ui(value, P.y, 2);
    mpz_invert(value, value, p);

    mpz_mul(lambda, lambda, value);
    mpz_mod(lambda, lambda, p);

    // compute Rx
    mpz_powm_ui(Rx, lambda, 2, p);
    mpz_mul_ui(value, P.x, 2);
    mpz_sub(Rx, Rx, value);

    // compute Ry
    mpz_sub(value, P.x, Rx);
    mpz_mul(Ry, lambda, value);
    mpz_sub(Ry, Ry, P.y);

    // assign values
    mpz_mod(R->x, Rx, p);
    mpz_mod(R->y, Ry, p);

    // clears
    mpz_clears(lambda, Rx, Ry, value, NULL);

    return 0;
}

int multiple(Point *R, Point P, mpz_t k, mpz_t p, mpz_t a, mpz_t b) {
    // checking if P is on the curve
    if(!on_curve(P, p, a, b)){
        return -1;
    }

    Point B;
    mpz_inits(B.x, B.y, NULL);
    mpz_set(B.x, P.x);
    mpz_set(B.y, P.y);

    mpz_set_ui(R->x, 0);
    mpz_set_ui(R->y, 0);
    
    int l = mpz_sizeinbase(k,2);
    for(int i=0; i<l; i++){
        if(mpz_tstbit(k, i) == 1){
            int add = addition(R, *R, B, p, a, b);
            if(add != 0){
                mpz_clears(B.x, B.y, NULL);
                return -1;
            }
        }
        int doub = doubly(&B, B, p, a, b);
        if(doub != 0){
            mpz_clears(B.x, B.y, NULL);
            return -1;
        }
    }

    mpz_clears(B.x, B.y, NULL);
    
    return 0;
}