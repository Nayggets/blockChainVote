#ifndef __CRYPTO_H
#define __CRYPTO_H

#include <stdio.h>
#include <stdlib.h>
#include <gmp.h>
#include <openssl/ssl.h>



// Génération de Clef
void generate_keys(mpz_t n, mpz_t lambda, mpz_t g, mpz_t mu);

// encryptage
void encrypt(mpz_t c, mpz_t m, mpz_t n, mpz_t g);

// decryptage
void decrypt(mpz_t m, mpz_t c, mpz_t lambda, mpz_t mu, mpz_t n);
#endif