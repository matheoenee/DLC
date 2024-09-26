#include <stdio.h>
#include <gmp.h>

int main() {
    // Déclaration des variables GMP
    mpz_t a, b, sum;

    // Initialisation des variables
    mpz_init(a);
    mpz_init(b);
    mpz_init(sum);

    // Assignation des valeurs aux variables
    mpz_set_str(a, "123456789012345678901234567890", 10);  // Nombre en base 10
    mpz_set_str(b, "987654321098765432109876543210", 10);

    // Addition de a et b
    mpz_add(sum, a, b);

    // Affichage du résultat
    gmp_printf("La somme est : %Zd\n", sum);

    // Libération de la mémoire allouée
    mpz_clear(a);
    mpz_clear(b);
    mpz_clear(sum);

    return 0;
}
