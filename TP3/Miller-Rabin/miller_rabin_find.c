#include <stdio.h>
#include <stdlib.h>
#include <gmp.h>
#include "miller_rabin.h"

int main(int argc, char *argv[]) {
    if (argc != 4)
    {
        fprintf(stderr, "Usage : %s <size b> <bases t> <results r>\n", argv[0]);
        return 1;
    }

    printf("Miller-Rabin Primality Test\n\n");

    int t, r, b;
    b = atoi(argv[1]);
    t = atoi(argv[2]);
    r = atoi(argv[3]);

    printf("[+] bit length : %d\n", b);
    printf("[+] number of results : %d\n", r);
    printf("[+] number of bases : %d\n", t);

    mpz_t z_n;
    mpz_init(z_n);

    gmp_randstate_t prng;
    gmp_randinit_default(prng);
    gmp_randseed_ui(prng, time(NULL));

    printf("\nSearching %d prime numbers of %d bits...\n", r, b);
    for(int i=0; i<r; i++){
        do
        {
            mpz_urandomb(z_n, prng, b);
            mpz_setbit(z_n, b - 1);
            if(test_miller_rabin(z_n, t) == 1){
                if(mpz_probab_prime_p(z_n, 5) == 0)
                    gmp_printf("[+] %Zu (false prime)\n",z_n);
                else
                    gmp_printf("[+] %Zu (true prime)\n",z_n);
            }
        } while(test_miller_rabin(z_n, t) != 1);
    }

    mpz_clear(z_n);
    gmp_randclear(prng);

    printf("End of programm.\n");
    return 0;
}