#include "add_and_double.h"
#include "struct.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int main(int argc, char *argv[]) {

    // mpz inits
    Point G, Q;
    mpz_t z_p, z_a, z_b, z_n, z_d;
    mpz_inits(z_p, z_a, z_b, z_n, z_d, G.x, G.y, Q.x, Q.y, NULL);

    FILE *file = fopen("params.txt", "r");
    if (!file) {
        fprintf(stderr, "Erreur : impossible d'ouvrir le fichier params.txt\n");
        return 1;
    }

    char line[256];
    while (fgets(line, sizeof(line), file)) {
        char *key = strtok(line, " =\n");
        char *value = strtok(NULL, " =\n");

        if (strcmp(key, "a") == 0) {
            mpz_set_str(z_a, value, 10);  // base 10 pour l'entrée
        } else if (strcmp(key, "b") == 0) {
            mpz_set_str(z_b, value, 10);
        } else if (strcmp(key, "p") == 0) {
            mpz_set_str(z_p, value, 10);
        } else if (strcmp(key, "Gx") == 0) {
            mpz_set_str(G.x, value, 10);
        } else if (strcmp(key, "Gy") == 0) {
            mpz_set_str(G.y, value, 10);
        } else if (strcmp(key, "n") == 0) {
            mpz_set_str(z_n, value, 10);
        }
    }

    fclose(file);

    // test functions
    printf("TP4 - Elliptic Curve DSA (ECDSA) - KeyGen\n");
    printf("y² = x³ + ax + b (mod p)\n\n");

    gmp_printf("[+] a = %Zu\n", z_a);
    gmp_printf("[+] b = %Zu\n", z_b);
    gmp_printf("[+] p = %Zu\n\n", z_p);

    printf("G = (Gx, Gy)\n");

    gmp_printf("[+] Gx = %Zu\n", G.x);
    gmp_printf("[+] Gy = %Zu\n", G.y);
    gmp_printf("[+] order of G = %Zu\n\n", z_n);

    printf("Checking if G is on the curve...\n");
    if(on_curve(G, z_p, z_a, z_b)) {
        printf("[+] G is on the curve!\n\n");
    } else {
        printf("[-] G is not on the curve!\n\n");
    }

    gmp_randstate_t prng;
    gmp_randinit_default(prng);
    gmp_randseed_ui(prng, time(NULL));

    mpz_urandomm(z_d, prng, z_n);

    printf("Generating random d ...\n");
    gmp_printf("[+] d   = 0x%Zx\n\n", z_d);

    printf("Computing Q = d.G ...\n");

    // Normal k.G
    multiple(&Q, G, z_d, z_p, z_a, z_b);

    gmp_printf("[+] Q_x = 0x%Zx\n", Q.x);
    gmp_printf("[+] Q_y = 0x%Zx\n\n", Q.y);

    printf("Checking if Q is on the curve...\n");
    if(on_curve(Q, z_p, z_a, z_b)) {
        printf("[+] Q is on the curve!\n\n");
    } else {
        printf("[-] Q is not on the curve!\n\n");
    }

    printf("[+] wrinting private and public key inside 'ecdsa.key'...\n");

    // Open the file in write mode and redirect stdout to it
    freopen("ecdsa.key", "w", stdout);

    gmp_printf("d=%Zx\n", z_d);
    gmp_printf("Qx=%Zx\n", Q.x);
    gmp_printf("Qy=%Zx\n", Q.y);

    fclose(stdout);

    // mpz clears
    mpz_clears(z_p, z_a, z_b, z_n, G.x, G.y, Q.x, Q.y, NULL);

    return 0;
}
