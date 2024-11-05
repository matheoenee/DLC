#include "read_data.h"

// Read elliptic curve parameters from a given file (a,b,p,Gx,Gy,n)
void read_parameters(const char *filename, mpz_t *a, mpz_t *b, mpz_t *p, mpz_t *Gx, mpz_t *Gy, mpz_t *n) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "[error] : failed to open %s\n", filename);
        exit(1);
    }

    char line[256];
    while (fgets(line, sizeof(line), file)) {
        char *key = strtok(line, " =\n");
        char *value = strtok(NULL, " =\n");

        if (strcmp(key, "a") == 0) {
            mpz_set_str(*a, value, 10);  // base 10 pour l'entrée
        } else if (strcmp(key, "b") == 0) {
            mpz_set_str(*b, value, 10);
        } else if (strcmp(key, "p") == 0) {
            mpz_set_str(*p, value, 10);
        } else if (strcmp(key, "Gx") == 0) {
            mpz_set_str(*Gx, value, 10);
        } else if (strcmp(key, "Gy") == 0) {
            mpz_set_str(*Gy, value, 10);
        } else if (strcmp(key, "n") == 0) {
            mpz_set_str(*n, value, 10);
        }
    }
}

// Read private key from a given file (d)
void read_private_key(const char *filename, mpz_t *d) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "[error] : failed to open %s\n", filename);
        exit(1);
    }

    char line[256];
    while (fgets(line, sizeof(line), file)) {
        char *key = strtok(line, " =\n");
        char *value = strtok(NULL, " =\n");

        if (strcmp(key, "d") == 0) {
            mpz_set_str(*d, value, 16);
        }
    }

    fclose(file);
}


// Read public key from a given file (Qx,Qy)
void read_public_key(const char *filename, mpz_t *Qx, mpz_t *Qy) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "[error] : failed to open %s\n", filename);
        exit(1);
    }

    char line[256];
    while (fgets(line, sizeof(line), file)) {
        char *key = strtok(line, " =\n");
        char *value = strtok(NULL, " =\n");

        if (strcmp(key, "Qx") == 0) {
            mpz_set_str(*Qx, value, 16);
        } else if (strcmp(key, "Qy") == 0) {
            mpz_set_str(*Qy, value, 16);
        }
    }

    fclose(file);
}

// Read signature from a given file (r,s)
void read_signature(const char *filename, mpz_t *r, mpz_t *s) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "[error] : failed to open %s\n", filename);
        exit(1);
    }

    char line[256];
    while (fgets(line, sizeof(line), file)) {
        char *key = strtok(line, " =\n");
        char *value = strtok(NULL, " =\n");

        if (strcmp(key, "r") == 0) {
            mpz_set_str(*r, value, 16);
        } else if (strcmp(key, "s") == 0) {
            mpz_set_str(*s, value, 16);
        }
    }

    fclose(file);
}