#ifndef __CRYPTO_H
#define __CRYPTO_H

#include <stdio.h>
#include <stdlib.h>
#include <gmp.h>

#include <stdio.h>
#include <stdlib.h>
#include <gmp.h>
#include "../include/crypto.h"

// Key generation
void generate_keys(mpz_t n, mpz_t lambda, mpz_t g, mpz_t mu);

// Encryption
void encrypt(mpz_t c, mpz_t m, mpz_t n, mpz_t g);

// Decryption
void decrypt(mpz_t m, mpz_t c, mpz_t lambda, mpz_t mu, mpz_t n);
#endif