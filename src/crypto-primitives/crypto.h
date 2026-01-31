/*
 * Cryptographic Primitives - Header File
 * 
 * LEARNING OBJECTIVES:
 * - Understand fundamental cryptographic operations
 * - Learn about symmetric encryption (AES)
 * - Explore cryptographic hash functions (SHA-256)
 * - Practice bit manipulation and mathematical operations
 * - Understand security principles and best practices
 * 
 * CONCEPTS COVERED:
 * - Block ciphers and encryption modes
 * - Substitution-Permutation Networks (SPN)
 * - Hash functions and message digests
 * - Key expansion and round functions
 * - Cryptographic constants and S-boxes
 * 
 * SECURITY NOTE:
 * This is an educational implementation. DO NOT use in production!
 * Use established libraries like OpenSSL for real applications.
 */

#ifndef CRYPTO_H
#define CRYPTO_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/*
 * AES (Advanced Encryption Standard) Constants
 * 
 * AES is a symmetric block cipher that operates on 128-bit blocks.
 * It supports key sizes of 128, 192, and 256 bits.
 */
#define AES_BLOCK_SIZE 16      // 128 bits = 16 bytes
#define AES_KEY_SIZE_128 16    // 128 bits = 16 bytes
#define AES_KEY_SIZE_192 24    // 192 bits = 24 bytes  
#define AES_KEY_SIZE_256 32    // 256 bits = 32 bytes
#define AES_ROUNDS_128 10      // Number of rounds for AES-128
#define AES_ROUNDS_192 12      // Number of rounds for AES-192
#define AES_ROUNDS_256 14      // Number of rounds for AES-256

/*
 * SHA-256 Constants
 * 
 * SHA-256 produces a 256-bit (32-byte) hash digest.
 */
#define SHA256_DIGEST_SIZE 32  // 256 bits = 32 bytes
#define SHA256_BLOCK_SIZE 64   // 512 bits = 64 bytes

/*
 * AES Data Structures
 */
typedef struct {
    uint8_t key[AES_KEY_SIZE_256];     // Original key (up to 256 bits)
    uint8_t round_keys[15][16];        // Expanded round keys
    int key_size;                      // Key size in bytes
    int rounds;                        // Number of rounds
} aes_context_t;

/*
 * SHA-256 Data Structures
 */
typedef struct {
    uint32_t state[8];                 // Hash state (8 x 32-bit words)
    uint8_t buffer[SHA256_BLOCK_SIZE]; // Input buffer
    uint64_t bit_count;                // Total bits processed
    size_t buffer_len;                 // Current buffer length
} sha256_context_t;

/*
 * AES FUNCTIONS
 */

/**
 * Initialize AES context with a key
 * @param ctx: AES context to initialize
 * @param key: Encryption key
 * @param key_size: Key size in bytes (16, 24, or 32)
 * @return: 0 on success, -1 on error
 */
int aes_init(aes_context_t* ctx, const uint8_t* key, int key_size);

/**
 * Encrypt a single 16-byte block using AES
 * @param ctx: Initialized AES context
 * @param plaintext: 16-byte input block
 * @param ciphertext: 16-byte output block
 */
void aes_encrypt_block(const aes_context_t* ctx, const uint8_t* plaintext, uint8_t* ciphertext);

/**
 * Decrypt a single 16-byte block using AES
 * @param ctx: Initialized AES context
 * @param ciphertext: 16-byte input block
 * @param plaintext: 16-byte output block
 */
void aes_decrypt_block(const aes_context_t* ctx, const uint8_t* ciphertext, uint8_t* plaintext);

/**
 * Encrypt data using AES in ECB mode (for educational purposes only!)
 * @param ctx: Initialized AES context
 * @param input: Input data (must be multiple of 16 bytes)
 * @param output: Output buffer (same size as input)
 * @param length: Length of data in bytes
 */
void aes_encrypt_ecb(const aes_context_t* ctx, const uint8_t* input, uint8_t* output, size_t length);

/**
 * Decrypt data using AES in ECB mode
 * @param ctx: Initialized AES context
 * @param input: Input data (must be multiple of 16 bytes)
 * @param output: Output buffer (same size as input)
 * @param length: Length of data in bytes
 */
void aes_decrypt_ecb(const aes_context_t* ctx, const uint8_t* input, uint8_t* output, size_t length);

/*
 * AES INTERNAL FUNCTIONS (for learning)
 */

/**
 * AES key expansion - generates round keys from main key
 * @param ctx: AES context
 * @param key: Original key
 * @param key_size: Key size in bytes
 */
void aes_key_expansion(aes_context_t* ctx, const uint8_t* key, int key_size);

/**
 * AES SubBytes transformation - applies S-box substitution
 * @param state: 16-byte state array
 */
void aes_sub_bytes(uint8_t state[16]);

/**
 * AES ShiftRows transformation - shifts rows of state matrix
 * @param state: 16-byte state array
 */
void aes_shift_rows(uint8_t state[16]);

/**
 * AES MixColumns transformation - mixes columns using matrix multiplication
 * @param state: 16-byte state array
 */
void aes_mix_columns(uint8_t state[16]);

/**
 * Add round key to state (XOR operation)
 * @param state: 16-byte state array
 * @param round_key: 16-byte round key
 */
void aes_add_round_key(uint8_t state[16], const uint8_t round_key[16]);

/*
 * SHA-256 FUNCTIONS
 */

/**
 * Initialize SHA-256 context
 * @param ctx: SHA-256 context to initialize
 */
void sha256_init(sha256_context_t* ctx);

/**
 * Process data through SHA-256
 * @param ctx: SHA-256 context
 * @param data: Input data
 * @param length: Length of data in bytes
 */
void sha256_update(sha256_context_t* ctx, const uint8_t* data, size_t length);

/**
 * Finalize SHA-256 and get digest
 * @param ctx: SHA-256 context
 * @param digest: Output buffer for 32-byte hash
 */
void sha256_final(sha256_context_t* ctx, uint8_t digest[SHA256_DIGEST_SIZE]);

/**
 * Convenience function to hash data in one call
 * @param data: Input data
 * @param length: Length of data
 * @param digest: Output buffer for 32-byte hash
 */
void sha256_hash(const uint8_t* data, size_t length, uint8_t digest[SHA256_DIGEST_SIZE]);

/*
 * UTILITY FUNCTIONS
 */

/**
 * Convert bytes to hexadecimal string
 * @param bytes: Input byte array
 * @param length: Number of bytes
 * @param hex_str: Output hex string (must be at least length*2+1 bytes)
 */
void bytes_to_hex(const uint8_t* bytes, size_t length, char* hex_str);

/**
 * Convert hexadecimal string to bytes
 * @param hex_str: Input hex string
 * @param bytes: Output byte array
 * @param max_bytes: Maximum bytes to write
 * @return: Number of bytes written, -1 on error
 */
int hex_to_bytes(const char* hex_str, uint8_t* bytes, size_t max_bytes);

/**
 * Secure memory comparison (constant time)
 * @param a: First buffer
 * @param b: Second buffer
 * @param length: Number of bytes to compare
 * @return: 0 if equal, non-zero if different
 */
int secure_memcmp(const void* a, const void* b, size_t length);

/**
 * Generate random bytes (simple PRNG for educational use)
 * @param buffer: Output buffer
 * @param length: Number of random bytes to generate
 */
void generate_random_bytes(uint8_t* buffer, size_t length);

/**
 * Print bytes in hexadecimal format for debugging
 * @param label: Description label
 * @param bytes: Byte array to print
 * @param length: Number of bytes
 */
void print_hex(const char* label, const uint8_t* bytes, size_t length);

#endif // CRYPTO_H