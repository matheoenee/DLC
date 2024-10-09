#include <stdio.h>
#include <stdlib.h>
#include <gmp.h>
#include <time.h>

// Miller-Rabin Primality test of n for a given base a
// Return 1 if a is a Strong Witness
// Return 0 if a is a Strong Liar
int test_miller_rabin_base(mpz_t z_n, mpz_t z_a, int s, mpz_t z_r){
    mpz_t z_y, z_n_sub_1;
    mpz_inits(z_y, z_n_sub_1, NULL);

    mpz_sub_ui(z_n_sub_1, z_n, 1); // n-1
    mpz_powm(z_y, z_a, z_r, z_n); // y = a^r mod n
    
    // if (y != 1) and (y != n-1)
    if((mpz_cmp_ui(z_y, 1) != 0) && (mpz_cmp(z_y, z_n_sub_1) != 0)){
        int j = 1;
        while ((j <= s-1) && (mpz_cmp(z_y, z_n_sub_1) != 0))
        {
        mpz_powm_ui(z_y, z_y, 2, z_n); // y = y^2 mod n
            // if y = 1
            if(mpz_cmp_ui(z_y, 1) == 0){ 
                mpz_clear(z_y);
                mpz_clear(z_n_sub_1);
                return 0; // composite
            }
            j++;
        }
        if(mpz_cmp(z_y, z_n_sub_1) != 0){
            mpz_clear(z_y);
            mpz_clear(z_n_sub_1);
            return 0; // composite
        }
    }
    mpz_clear(z_y);
    mpz_clear(z_n_sub_1);
    return 1; // prime
}

// Miller-Rabin Test for t bases
// Return 0 if n is composite
// Return 1 if n is prime 
int test_miller_rabin(mpz_t z_n, int t){

    // Condition n > 3
    if (mpz_cmp_ui(z_n, 4) <= 0) {
        printf("   [Error] n should be greater than 3.\n");
        return 1;
    }

    // If n is even
    if(mpz_divisible_2exp_p(z_n, 1)){
        return 0;
    }

    gmp_randstate_t prng;
    gmp_randinit_default(prng);
    gmp_randseed_ui(prng, time(NULL));

    mpz_t z_max_bound, z_r, z_a;
    mpz_inits(z_max_bound, z_r, z_a, NULL);
    mpz_sub_ui(z_max_bound, z_n, 3);
    
    // Decomposition n-1 = 2^s * r (r odd)
    mpz_sub_ui(z_r, z_n, 1);
    int s = 0;
    while (mpz_divisible_2exp_p(z_r, 1)) {
        mpz_fdiv_q_2exp(z_r, z_r, 1);
        s++;
    }
    //gmp_printf("[+] %Zu - 1 = 2^%d * %Zu\n",z_n, s, z_r);

    for(int i=0; i<t; i++){
        // Get a random base a
        mpz_urandomm(z_a, prng, z_max_bound);
        mpz_add_ui(z_a, z_a, 2);

        if(test_miller_rabin_base(z_n, z_a, s, z_r) == 0){
            // clear GMP variables
            mpz_clear(z_max_bound);
            mpz_clear(z_a);
            mpz_clear(z_r);
            gmp_randclear(prng);
            return 0;
        }
    }

    // clear GMP variables
    mpz_clear(z_max_bound);
    mpz_clear(z_a);
    mpz_clear(z_r);
    gmp_randclear(prng);

    return 1;
}