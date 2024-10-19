#include "basic_sieve.h"
#include <stdio.h>
#include <sys/types.h>
#include <sys/resource.h>
#include <stdlib.h>
#include <time.h>

unsigned long int cputime() {
    struct rusage rus;
    getrusage(0, &rus);
    return rus.ru_utime.tv_sec * 1000 + rus.ru_utime.tv_usec / 1000;
}


int main(int argc, char *argv[]) {
    if (argc != 4)
    {
        fprintf(stderr, "Usage : %s <size b> <bases t> <results r>\n", argv[0]);
        return 1;
    }

    printf("Basic Sieve Prime Generation\n\n");

    int t, r, k, b;
    b = atoi(argv[1]);
    t = atoi(argv[2]);
    r = atoi(argv[3]);

    printf("[+] bit length : %d\n", b);
    printf("[+] number of results : %d\n", r);
    printf("[+] number of bases : %d\n", t);

    mpz_t z_p;
    mpz_init(z_p);

    gmp_randstate_t prng;
    gmp_randinit_default(prng);
    gmp_randseed_ui(prng, time(NULL));

    printf("\nGenerating %d prime numbers of %d bits...\n", r, b);

    // redirect stdout into "output.txt"
    freopen("output.txt", "w", stdout);

    for(int k = 5; k <= 100; k = k+5){
        //get t1 (start time)
        long int t1 = cputime();

        // generate the first k primes number
        int* primes = get_k_first_prime(k);

        for(int i=0; i<r; i++){
            mpz_urandomb(z_p, prng, b);
            mpz_setbit(z_p, b - 1);
            mpz_setbit(z_p, 0);
                
            basic_sieve_prime_gen(z_p, b, k, primes, t);
            //gmp_printf("[+] p_%d = %Zu\n", i, z_p);
        }

        // get t2 (end time)
        int long t2 = cputime();
        printf("%d %d\n",k, t2-t1);

        free(primes);
    }

    mpz_clear(z_p);
    gmp_randclear(prng);

    //printf("End of programm.\n");
    return 0;
}