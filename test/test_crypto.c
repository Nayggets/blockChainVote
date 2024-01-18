#include "../common/include/crypto.h"

int main()
{
    mpz_t n, lambda, g, mu, m, c, sum_encrypted;
    mpz_inits(n, lambda, g, mu, m, c, NULL);
    mpz_inits(sum_encrypted, NULL);
    // Generate keys
    generate_keys(n, lambda, g, mu);

    // Encrypt a message (e.g., 42)
    mpz_set_ui(m, 1);
    encrypt(c, m, n, g);
    gmp_printf("Encrypted message: %Zd\n", c);

    // mpz_set(sum_encrypted, c);
    mpz_mul(sum_encrypted, c, c);             //+2
    mpz_mul(sum_encrypted, sum_encrypted, c); //+1
    mpz_mul(sum_encrypted, sum_encrypted, c); //+1

    // Decrypt the message
    decrypt(m, sum_encrypted, lambda, mu, n);
    gmp_printf("Decrypted message : %Zd\n", m);

    return 0;
}