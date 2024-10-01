#include <stdio.h>
#include <gmp.h>

void decompose_base256(mpz_t n) {
    mpz_t byte, temp;
    mpz_init(byte);
    mpz_init(temp);
    int i = 0;

    while (mpz_cmp_ui(n, 0) > 0) {
        // Récupérer le dernier octet (les 8 bits de poids faible)
        mpz_mod_ui(byte, n, 256);
        
        // Afficher cet octet
        gmp_printf("Byte %d: %Zd\n", i, byte);
        
        // Diviser n par 256 pour passer au prochain octet
        mpz_fdiv_q_ui(n, n, 256);
        i++;
    }

    mpz_clear(byte);
    mpz_clear(temp);
}

int main() {
    mpz_t number;
    mpz_init(number);

    // Initialiser le nombre avec une grande valeur
    mpz_set_str(number, "36131812707232880684007142933947836718260166545365746704933704950795381578658", 10);
    mpz_set_str(number, "4186651671651506119521186883657377146838637476293330068172612244937634", 10);

    

    // Appeler la fonction de décomposition
    decompose_base256(number);

    mpz_clear(number);
    return 0;
}
