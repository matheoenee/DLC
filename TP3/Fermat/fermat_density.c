#include <stdio.h>
#include <stdlib.h>
#include <gmp.h>
#include "fermat.h"

int main(int argc, char *argv[]) {
    if (argc != 4)
    {
        fprintf(stderr, "Usage : %s <size b> <bases t> <test r>\n", argv[0]);
        return 1;
    }

    printf("Fermat Primality Test\n\n");

    int t, r, b;
    b = atoi(argv[1]);
    t = atoi(argv[2]);
    r = atoi(argv[3]);

    printf("[+] bit length : %d\n", b);
    printf("[+] number of tests : %d\n", r);
    printf("[+] number of bases : %d\n", t);

    mpz_t z_n;
    mpz_init(z_n);

    gmp_randstate_t prng;
    gmp_randinit_default(prng);
    gmp_randseed_ui(prng, time(NULL));

    int count_prime = 0;

    printf("\nTesting %d numbers of %d bit length...\n", r, b);

    for(int i=0; i<r; i++){
        mpz_urandomb(z_n, prng, b);
        mpz_setbit(z_n, b - 1);

        if(test_fermat(z_n, t) == 1){
            count_prime++;
        }
    }

    printf("[Result] %d prime number(s) over %d of %d bits.\n",count_prime, r, b);
    
    mpz_clear(z_n);
    gmp_randclear(prng);

    printf("End of programm.\n");
    return 0;
}