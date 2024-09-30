#include <stdio.h>
#include <stdlib.h>
#include <gmp.h>
#include <time.h>
#include <stdio.h>
#include <string.h>

void decrypt_rsa(mpz_t z_m, mpz_t z_c, mpz_t z_d, mpz_t z_n){
    if(mpz_cmp(z_c,z_n) > 0){
        printf("[error] c > n\n");
        return;
    }
    mpz_powm(z_m, z_c, z_d, z_n);
}

int main(int argc, char *argv[]) {
    if (argc != 4)
    {
        fprintf(stderr, "Usage : %s <input file> <output file> <key file>\n", argv[0]);
        return 1;
    }

    printf("RSA File Decryption (Standard mode)\n\n");

    FILE *input_file;
    FILE *output_file;
    FILE *key_file;

    // GMP variables initialization
    mpz_t z_d, z_n, z_e; // RSA standard variables
    mpz_inits(z_d, z_n, z_e, NULL);

    // Reading RSA components from key file
    printf("Reading data from key file...\n");
    key_file = fopen(argv[3],"r");
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

    // Decryption
    printf("Starting decryption...\n");

    input_file = fopen(argv[1], "rb");
    if (!input_file) {
        perror("[Error] failed to open input file.\n");
        return 1;
    }

    output_file = fopen(argv[2], "w");
    if (!output_file) {
        perror("[Error] failed to create output file.\n");
        return 1;
    }

    printf("[+] files : OK\n");

    // Computing maximum size of bytes block so that m < n
    // 2^9 * 256^max < 2^n-1 
    int max = (mpz_sizeinbase(z_n, 2))/9 + 1;
    printf("[+] Maximum size of bytes block is : %d\n", max);

    mpz_t z_m, z_c, z_a, z_b;
    mpz_inits(z_m, z_c, z_a, z_b, NULL);
    unsigned char byte;
    int i = 0;
    int block = 0;

    while (fread(&byte, 1, 1, input_file) == 1) {
        if(i == 0) printf("\n[+] Reading block %d\n", block);

        // If max block size is not reached
        //printf("   [+] Reading new byte... (%d)\n", byte);
        // Computing c by performing base 256 transformation on bytes
        // c = c + b with b = a * (byte_i in base 10) where a = 256^i
        mpz_ui_pow_ui(z_a, 256, i);
        mpz_mul_ui(z_b, z_a, (int)byte);
        mpz_add(z_c, z_c, z_b);
        gmp_printf("   [info] i = %d | a = %Zu | b = %d\n",i, z_a, byte);
        i++;

        // Max block size is reached
        if(i > max)
        {
            printf("   [+] Writting block %d to output file...\n", block);
            if(mpz_cmp(z_c, z_n)>0){
                printf("[ALERT] c > n\n");
            }
            decrypt_rsa(z_m, z_c, z_d, z_n); // c decryption
            gmp_printf("   [info] block %d :\n   c = %Zu\n   m = %Zu\n", block, z_c, z_m);
            while (i > 0)
            {
                i--;
                // Computing the inverse operation 
                // (byte_i in base 10) = m / 256^i mod 256
                mpz_ui_pow_ui(z_a, 256, max-i);
                mpz_fdiv_q(z_b, z_m, z_a);
                mpz_mod_ui(z_b, z_b, 256);
                byte = mpz_get_ui(z_b);
                gmp_printf("   [info] i = %d | a = %Zu | b = %d\n",i, z_a, byte);
                fwrite(&byte, 1, 1, output_file);
            }
            // Reset c for next block
            mpz_set_ui(z_c, 0);
            block++;
        }
    }

    // Last block decryption
    decrypt_rsa(z_m, z_c, z_e, z_n); // c decryption
    gmp_printf("   [info] block %d :\n   c = %Zu\n   m = %Zu\n", block, z_c, z_m);
    while (i > 0)
    {
        i--;
        // Computing the inverse operation 
        // (byte_i in base 10) = m / 256^i mod 256
        mpz_ui_pow_ui(z_a, 256, max-i);
        mpz_fdiv_q(z_b, z_m, z_a);
        mpz_mod_ui(z_b, z_b, 256);
        byte = mpz_get_ui(z_b);
        gmp_printf("   [info] i = %d | a = %Zu | b = %d\n",i, z_a, byte);
        fwrite(&byte, 1, 1, output_file);
    }
    block++;

    printf("[+] End of decryption...\n");

    // Clear GMP encryption variables
    mpz_clears(z_m, z_c, z_a, z_b, NULL);
    fclose(input_file);
    fclose(output_file);

    printf("[+] %s successfully decrypted %s in %s!\n", argv[1], argv[2]);
    
    // Clear GMP variables
    mpz_clears(z_d, z_e, z_n, NULL);
    
    printf("\nEnd of program.\n");
    return 0;
}