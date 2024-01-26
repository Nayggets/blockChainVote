#include <stdio.h>
#include <stdlib.h>
#include <gmp.h>

#include "../common/include/crypto.h"

// Key generation
void generate_keys(mpz_t n, mpz_t lambda, mpz_t g, mpz_t mu)
{
    mpz_t x, p, q, n_square;
    mpz_inits(x, p, q, n, lambda, n_square, NULL);

    // Generate two large prime numbers p and q
    mpz_set_str(p, "61", 10);
    mpz_set_str(q, "53", 10);

    // Compute n = p * q
    mpz_mul(n, p, q);

    // Compute lambda = lcm(p-1, q-1)
    mpz_sub_ui(p, p, 1);
    mpz_sub_ui(q, q, 1);
    mpz_lcm(lambda, p, q);

    // Compute n^2
    mpz_mul(n_square, n, n);
    // Choose g as n + 1
    mpz_set(g, n);
    mpz_add_ui(g, g, 1);

    // Compute mu = (L(g^lambda mod n^2))^-1 mod n
    mpz_powm(x, g, lambda, n_square);
    mpz_sub_ui(x, x, 1);
    mpz_div(x, x, n);
    mpz_invert(mu, x, n);
}

// Encryption
void encrypt(mpz_t c, mpz_t m, mpz_t n, mpz_t g)
{
    mpz_t r, n_square;
    mpz_inits(r, n_square, NULL);

    // Generate random number r
    mpz_set_ui(r, 17);

    // Compute n^2
    mpz_mul(n_square, n, n);

    // Compute c = g^m * r^n mod n^2
    mpz_powm(c, g, m, n_square);
    mpz_powm(r, r, n, n_square);
    mpz_mul(c, c, r);
    mpz_mod(c, c, n_square);
}

// Decryption
void decrypt(mpz_t m, mpz_t c, mpz_t lambda, mpz_t mu, mpz_t n)
{
    mpz_t n_square, temp;
    mpz_inits(n_square, temp, NULL);

    // Compute n^2
    mpz_mul(n_square, n, n);

    // Compute m = L(c^lambda mod n^2) * mu mod n
    mpz_powm(temp, c, lambda, n_square);
    mpz_sub_ui(temp, temp, 1);
    mpz_div(temp, temp, n);
    mpz_mul(m, temp, mu);
    mpz_mod(m, m, n);
}