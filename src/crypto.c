#include <stdio.h>
#include <stdlib.h>
#include <gmp.h>

#include "../common/include/crypto.h"

// Generation des clefs
void generate_keys(mpz_t n, mpz_t lambda, mpz_t g, mpz_t mu)
{
    mpz_t x, p, q, n_square;
    mpz_inits(x, p, q, n, lambda, n_square, NULL);

    // genere deux nombres premiers p et q
    mpz_set_str(p, "61", 10);
    mpz_set_str(q, "53", 10);


    // Calculer n = p * q
    mpz_mul(n, p, q);

    // Calculer lambda = lcm(p-1, q-1)
    mpz_sub_ui(p, p, 1);
    mpz_sub_ui(q, q, 1);
    mpz_lcm(lambda, p, q);

    // Calculer n^2
    mpz_mul(n_square, n, n);
    // choisi g comme étant n + 1
    mpz_set(g, n);
    mpz_add_ui(g, g, 1);

    // Calculer mu = (L(g^lambda mod n^2))^-1 mod n
    mpz_powm(x, g, lambda, n_square);
    mpz_sub_ui(x, x, 1);
    mpz_div(x, x, n);
    mpz_invert(mu, x, n);
}

// Encryptage
void encrypt(mpz_t c, mpz_t m, mpz_t n, mpz_t g)
{
    mpz_t r, n_square;
    mpz_inits(r, n_square, NULL);


    // Generer un nombre aleatoire r
    mpz_set_ui(r, 17);


    // Calculer n^2
    mpz_mul(n_square, n, n);

    // Calculer c = g^m * r^n mod n^2
    mpz_powm(c, g, m, n_square);
    mpz_powm(r, r, n, n_square);
    mpz_mul(c, c, r);
    mpz_mod(c, c, n_square);
}

// Decryptage
void decrypt(mpz_t m, mpz_t c, mpz_t lambda, mpz_t mu, mpz_t n)
{
    mpz_t n_square, temp;
    mpz_inits(n_square, temp, NULL);

    // Calculer n^2
    mpz_mul(n_square, n, n);

    // Calculer m = L(c^lambda mod n^2) * mu mod n
    mpz_powm(temp, c, lambda, n_square);
    mpz_sub_ui(temp, temp, 1);
    mpz_div(temp, temp, n);
    mpz_mul(m, temp, mu);
    mpz_mod(m, m, n);
}