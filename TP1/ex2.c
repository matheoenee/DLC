#include <stdio.h>
#include <stdlib.h>
#include <gmp.h>
#include <time.h>

int main(int argc, char *argv[]) {
    if (argc < 2)
    {
        fprintf(stderr, "Usage : %s k seed\n", argv[0]);
        return 1;
    }

    printf("TP1 - Exercice 1 \"PRNG\"\n");

    int k = atoi(argv[1]);
    int seed;
    if(argc == 3){
        seed = atoi(argv[2]);
    }else{
        seed = time(NULL);
    }

    mpz_t z_random;
    gmp_randstate_t prng;

    mpz_init(z_random);

    gmp_randinit_default(prng);
    printf("[info] seed = %d\n\n",seed);
    gmp_randseed_ui(prng, seed);

    // CONDITION : is primer number + bit length condition
    /*

    mpz_t z_inf;
    mpz_init(z_inf);
    mpz_set_ui(z_inf,1);
    mpz_mul_2exp(z_inf, z_inf, k-1);

    gmp_printf("[+] Inf : %Zu\n",z_inf);

    while (mpz_probab_prime_p(z_random,5) == 0)
    {
        mpz_urandomb(z_random, prng, k);
        while(mpz_cmp(z_random,z_inf)<0)
        {
            mpz_urandomb(z_random, prng, k);
        }
        char *binary_representation = mpz_get_str(NULL, 2, z_random);
        gmp_printf("[+] Random : %Zu (%s)\n",z_random,binary_representation);
    }

    mpz_clear(z_inf);
    */

    // CONDITION : is primer number + decimal length condition
    mpz_t z_sup;
    mpz_init(z_sup);
    mpz_ui_pow_ui(z_sup,10,k);

    mpz_t z_inf;
    mpz_init(z_inf);
    mpz_set_ui(z_inf,10);
    mpz_pow_ui(z_inf,z_inf,k-1);

    while (mpz_probab_prime_p(z_random,5) == 0)
    {
        mpz_urandomm(z_random, prng, z_sup);
        while(mpz_cmp(z_random,z_inf)<0)
        {
            mpz_urandomm(z_random, prng, z_sup);
        }
        char *binary_representation = mpz_get_str(NULL, 2, z_random);
        gmp_printf("[+] Random : %Zu (%s)\n",z_random,binary_representation);
    }

    mpz_clear(z_sup);
    mpz_clear(z_inf);
    
    
    printf("Fin du prorgamme.\n");
    gmp_randclear(prng);
    mpz_clear(z_random);

    return 0;
}