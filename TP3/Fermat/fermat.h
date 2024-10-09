#include <stdio.h>
#include <stdlib.h>
#include <gmp.h>
#include <time.h>

// Test base a for integer n
// Return 1 if a is a Fermat Witness
// Return 0 if a is a Fermat Liar
int test_fermat_base(mpz_t z_n, mpz_t z_a){
    int witness = 1;
    mpz_t z_test, z_ord;
    mpz_inits(z_test, z_ord, NULL);

    mpz_sub_ui(z_ord, z_n, 1);

    mpz_powm(z_test, z_a, z_ord, z_n);
    if(mpz_cmp_ui(z_test, 1) == 0){
        witness = 0;
    }
    
    mpz_clear(z_test);
    mpz_clear(z_ord);
    return witness;
}

// Fermat Test for t bases for integer n
// Return 0 if n is composed
// Return 1 if n is pseudoprime in the t bases 
int test_fermat(mpz_t z_n, int t){

    // Condition n > 3
    if (mpz_cmp_ui(z_n, 4) <= 0) {
        printf("   [Error] n should be greater than 3.\n");
        return 1;
    }

    gmp_randstate_t prng;
    gmp_randinit_default(prng);
    gmp_randseed_ui(prng, time(NULL));

    mpz_t z_max_bound, z_a;
    mpz_inits(z_max_bound, z_a, NULL);
    mpz_sub_ui(z_max_bound, z_n, 3);

    for(int i=0; i<t; i++){
        mpz_urandomm(z_a, prng, z_max_bound);
        mpz_add_ui(z_a, z_a, 2);
        if(test_fermat_base(z_n, z_a) == 1){
            mpz_clear(z_max_bound);
            mpz_clear(z_a);
            gmp_randclear(prng);

            return 0;
        }
    }

    mpz_clear(z_max_bound);
    mpz_clear(z_a);
    gmp_randclear(prng);

    return 1;
}