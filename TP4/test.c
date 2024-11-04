#include "add_and_double.h"
#include "struct.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*

Info : Voici les coordonnées de quelques points de la courbe pour vous aider dans votre debug.

(2.G).x = 5369744403678710563432458361254544170966096384586764429448
(2.G).y = 5429234379789071039750654906915254128254326554272718558123

(3.G).x = 2915109630280678890720206779706963455590627465886103135194
(3.G).y = 2946626711558792003980654088990112021985937607003425539581

(25.G).x = 4050458003911261006476221652143368703341770023851540414820
(25.G).y = 1462996413535013317017641479160615535560398926517444701794

*/

int main(int argc, char *argv[]) {

    if (argc < 2) {
        fprintf(stderr, "Usage: %s <k>\n", argv[0]);
        return 1;
    }

    // mpz inits
    Point G, R;
    mpz_t z_p, z_a, z_b, z_n, z_k;
    mpz_inits(z_p, z_a, z_b, z_n, z_k, G.x, G.y, R.x, R.y, NULL);

    mpz_set_str(z_k, argv[1], 10);

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
    printf("TP4 - Elliptic Curve Cryptography (ECC)\n");
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

    // Testing multiple
    gmp_printf("Testing mutpiple... (R = %Zu.P)\n", z_k);

    // Normal k.G
    int mul = multiple(&R, G, z_k, z_p, z_a, z_b);
    if(mul == 0){
        printf("[+] successful multiple!\n");
    } else {
        printf("[+] failed multiple!\n");
    }
    gmp_printf("[+] Rx = %Zu\n", R.x);
    gmp_printf("[+] Ry = %Zu\n\n", R.y);

    if(mpz_even_p(z_k)){
        // Doubly of (k/2).G
        mpz_tdiv_q_ui(z_k, z_k, 2);
        mul = multiple(&R, G, z_k, z_p, z_a, z_b);
        int doub = doubly(&R, R, z_p, z_a, z_b);
        if(mul == 0 && doub == 0){
            printf("[+] successful multiple and doubly!\n");
        } else {
            printf("[+] failed multiple or doubly!\n");
        }
        gmp_printf("[+] Rx = %Zu\n", R.x);
        gmp_printf("[+] Ry = %Zu\n\n", R.y);

        // (k-1).G + G
        mpz_mul_ui(z_k, z_k, 2);
        mpz_sub_ui(z_k, z_k, 1);
        mul = multiple(&R, G, z_k, z_p, z_a, z_b);
        int add = addition(&R, R, G, z_p, z_a, z_b);
        if(mul == 0 && add == 0){
            printf("[+] successful multiple and add!\n");
        } else {
            printf("[+] failed multiple or add!\n");
        }
        gmp_printf("[+] Rx = %Zu\n", R.x);
        gmp_printf("[+] Ry = %Zu\n\n", R.y);
    }

    printf("Checking if R is on the curve...\n");
    if(on_curve(R, z_p, z_a, z_b)) {
        printf("[+] R is on the curve!\n\n");
    } else {
        printf("[-] R is not on the curve!\n\n");
    }

    // mpz clears
    mpz_clears(z_p, z_a, z_b, z_n, G.x, G.y, R.x, R.y, NULL);

    return 0;
}
