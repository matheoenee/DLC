#include <stdio.h>
#include <stdlib.h>
#include <gmp.h>
#include <time.h>
#include <stdio.h>
#include <string.h>

void encrypt_rsa(mpz_t z_c, mpz_t z_m, mpz_t z_e, mpz_t z_n){
    if(mpz_cmp(z_m,z_n) > 0){
        printf("[error] m > n\n");
        return;
    }
    mpz_powm(z_c, z_m, z_e, z_n);
}

void decrypt_rsa(mpz_t z_m, mpz_t z_c, mpz_t z_d, mpz_t z_n){
    if(mpz_cmp(z_c,z_n) > 0){
        printf("[error] c > n\n");
        return;
    }
    mpz_powm(z_m, z_c, z_d, z_n);
}

int main(int argc, char *argv[]) {
    if (argc != 2)
    {
        fprintf(stderr, "Usage : %s <key file>\n", argv[0]);
        return 1;
    }

    printf("RSA Key Test (Standard mode)\n\n");

    FILE *key_file;

    // GMP variables initialization
    mpz_t z_d, z_n, z_e; // RSA standard variables
    mpz_inits(z_d, z_n, z_e, NULL);

    // Reading RSA components from key file
    printf("Reading data from key file...\n");
    key_file = fopen(argv[1],"r");
    if (!key_file) {
        perror("[Error] failed to open key file.\n");
        return 1;
    }

    // Variables pour stocker les lignes lues
    char buffer[1024];

    // Lire les valeurs du fichier
    while (fgets(buffer, sizeof(buffer), key_file)) {
        if (sscanf(buffer, "e = 0x%1023s", buffer) == 1) {
            mpz_set_str(z_e, buffer, 16);  // Charger e en base 16
            printf("[+] reading value of e...\n");
            printf("   e = 0x%s\n", mpz_get_str(NULL, 16, z_e));
            printf("   e = %s\n", mpz_get_str(NULL, 10, z_e));
            printf("   lenght : %d bits\n",mpz_sizeinbase(z_e,2));
        } else if (sscanf(buffer, "n = 0x%1023s", buffer) == 1) {
            mpz_set_str(z_n, buffer, 16);  // Charger n en base 16
            printf("[+] reading value of n...\n");
            printf("   n = 0x%s\n", mpz_get_str(NULL, 16, z_n));
            printf("   lenght : %d bits\n",mpz_sizeinbase(z_n,2));
        } else if (sscanf(buffer, "d = 0x%1023s", buffer) == 1) {
            mpz_set_str(z_d, buffer, 16);  // Charger d en base 16
            printf("[+] reading value of d...\n");
            printf("   d = 0x%s\n", mpz_get_str(NULL, 16, z_d));
            printf("   lenght : %d bits\n",mpz_sizeinbase(z_d,2));
        }
    }

    fclose(key_file);

    mpz_t z_m1, z_m2, z_c; // RSA test variables
    mpz_inits(z_m1, z_m2, z_c, NULL);

    mpz_set_ui(z_m1, 44903392628);
    encrypt_rsa(z_c, z_m1, z_e, z_n);
    decrypt_rsa(z_m2, z_c, z_d, z_n);

    gmp_printf("m1 = %Zu\n",z_m1);
    gmp_printf("c  = %Zu\n",z_c);
    gmp_printf("m2 = %Zu\n",z_m2);

    mpz_clears(z_m1, z_m2, z_c, NULL);
    mpz_clears(z_e, z_d, z_n, NULL);

    return 0;
}