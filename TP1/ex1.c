#include <stdio.h>
#include <stdlib.h>
#include <gmp.h>
#include <time.h>

int main(int argc, char *argv[]) {
    if (argc != 3)
    {
        fprintf(stderr, "Usage : %s k seed\n", argv[0]);
        return 1;
    }

    printf("TP1 - Exercice 1 \"PRNG\"\n");

    int k = atoi(argv[1]);
    int seed = atoi(argv[2]);

    mpz_t z_random;
    gmp_randstate_t prng;

    mpz_init(z_random);

    gmp_randinit_default(prng);
    printf("[info] seed = %d\n\n",seed);
    gmp_randseed_ui(prng, seed);

    // CONDITION : is multiple of 20
    /*
    mpz_t z_remainder;
    mpz_init(z_remainder);
    mpz_set_ui(z_remainder,1);

    while (mpz_cmp_ui(z_remainder,0) != 0)
    {
        mpz_urandomb(z_random, prng, k);
        gmp_printf("[+] Random : %Zu\n",z_random);
        mpz_mod_ui(z_remainder,z_random,20);
    }

    mpz_clear(z_remainder);
    */

   // CONDITION : is primer number
   while (mpz_probab_prime_p(z_random,5) == 0)
    {
        mpz_urandomb(z_random, prng, k);
        gmp_printf("[+] Random : %Zu\n",z_random);
    }
    
    
    
    printf("Fin du prorgamme.\n");
    gmp_randclear(prng);
    mpz_clear(z_random);

    return 0;
}