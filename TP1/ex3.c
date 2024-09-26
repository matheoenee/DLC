#include <stdio.h>
#include <stdlib.h>
#include <gmp.h>
#include <time.h>

int main(int argc, char *argv[]) {

    // Read values from files
    FILE *file;
    char n_str[1024], d_str[1024], c_str[1024], m_str[1024];
    mpz_t z_n, z_d, z_c, z_m;

    mpz_inits(z_n,z_d,z_c,z_m,NULL);

    file = fopen("private_key.txt","r");
    if (file == NULL) {
        printf("Erreur lors de l'ouverture du fichier.\n");
        return 1;
    }

    fscanf(file, "n = %1023s\n", n_str);
    fscanf(file, "d = %1023s\n", d_str);
    fclose(file);

    file = fopen("plaintext.txt","r");
    if (file == NULL) {
        printf("Erreur lors de l'ouverture du fichier.\n");
        return 1;
    }

    fscanf(file, "m = %1023s\n", m_str);
    fclose(file);

    file = fopen("ciphertext.txt","r");
    if (file == NULL) {
        printf("Erreur lors de l'ouverture du fichier.\n");
        return 1;
    }

    fscanf(file, "c = %1023s\n", c_str);
    fclose(file);

    mpz_set_str(z_n, n_str, 16);
    mpz_set_str(z_d, d_str, 16);
    mpz_set_str(z_m, m_str, 16);
    mpz_set_str(z_c, c_str, 16);

    // Afficher les résultats
    gmp_printf("n = %Zu\n", z_n);
    gmp_printf("d = %Zu\n", z_d);
    gmp_printf("m = %Zu\n", z_m);
    gmp_printf("c = %Zu\n", z_c);


    mpz_t z_m2;
    mpz_init(z_m2);

    // Modular exponentiation with GMP
    // mpz_powm(z_m2, z_c, z_d, z_n);

    int k = mpz_sizeinbase(z_d,2);

    //Right-to-left method
    /* mpz_t z_a, z_b;
    mpz_inits(z_a,z_b,NULL);
    mpz_set_ui(z_a, 1);
    mpz_set(z_b,z_c);
    
    for(int i=0; i<k; i++){
        if(mpz_tstbit(z_d, i) == 1){
            mpz_mul(z_a, z_a, z_b);
            mpz_mod(z_a,z_a,z_n);
        }
        mpz_mul(z_b,z_b,z_b);
        mpz_mod(z_b,z_b,z_n);
    }
    mpz_set(z_m2, z_a);
    mpz_clears(z_a,z_b,NULL);
    */

    //Left-to-rigth method
    mpz_t z_a;
    mpz_init(z_a);
    mpz_set_ui(z_a, 1);
    
    for(int i=k-1; i>=0; i--){
        mpz_mul(z_a,z_a,z_a);
        mpz_mod(z_a,z_a,z_n);
        if(mpz_tstbit(z_d, i) == 1){
            mpz_mul(z_a, z_a, z_c);
            mpz_mod(z_a,z_a,z_n);
        }
    }
    mpz_set(z_m2, z_a);
    mpz_clear(z_a);
    
    // Decipher test
    if(mpz_cmp(z_m, z_m2) == 0)
    {
        printf("[+] Congratulation, you have decrypted the message.\n");
    }else{
        printf("[+] Error, you failed.\n");
    }

    mpz_clear(z_n);
    mpz_clear(z_d);
    mpz_clear(z_m);
    mpz_clear(z_m2);
    mpz_clear(z_c);
}