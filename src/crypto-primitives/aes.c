/*
 * AES (Advanced Encryption Standard) Implementation
 * 
 * This is an educational implementation of AES-128/192/256.
 * It demonstrates the core concepts of modern block ciphers.
 * 
 * AES ALGORITHM OVERVIEW:
 * 1. Key Expansion - Generate round keys from main key
 * 2. Initial Round - AddRoundKey
 * 3. Main Rounds (9/11/13) - SubBytes, ShiftRows, MixColumns, AddRoundKey
 * 4. Final Round - SubBytes, ShiftRows, AddRoundKey (no MixColumns)
 * 
 * SECURITY WARNING: This is for educational purposes only!
 * Use established crypto libraries for production code.
 */

#include "crypto.h"
#include <stdio.h>
#include <string.h>

/*
 * AES S-BOX (Substitution Box)
 * 
 * The S-box is a lookup table used in the SubBytes transformation.
 * It provides non-linearity to the cipher, making it resistant to
 * linear and differential cryptanalysis.
 * 
 * Each byte is replaced by the value at S_BOX[byte].
 */
static const uint8_t S_BOX[256] = {
    // 0     1     2     3     4     5     6     7     8     9     A     B     C     D     E     F
    0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76,
    0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0,
    0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15,
    0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75,
    0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84,
    0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf,
    0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8,
    0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2,
    0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73,
    0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb,
    0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79,
    0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08,
    0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a,
    0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e,
    0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf,
    0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16
};

/*
 * AES INVERSE S-BOX (for decryption)
 */
static const uint8_t INV_S_BOX[256] = {
    // 0     1     2     3     4     5     6     7     8     9     A     B     C     D     E     F
    0x52, 0x09, 0x6a, 0xd5, 0x30, 0x36, 0xa5, 0x38, 0xbf, 0x40, 0xa3, 0x9e, 0x81, 0xf3, 0xd7, 0xfb,
    0x7c, 0xe3, 0x39, 0x82, 0x9b, 0x2f, 0xff, 0x87, 0x34, 0x8e, 0x43, 0x44, 0xc4, 0xde, 0xe9, 0xcb,
    0x54, 0x7b, 0x94, 0x32, 0xa6, 0xc2, 0x23, 0x3d, 0xee, 0x4c, 0x95, 0x0b, 0x42, 0xfa, 0xc3, 0x4e,
    0x08, 0x2e, 0xa1, 0x66, 0x28, 0xd9, 0x24, 0xb2, 0x76, 0x5b, 0xa2, 0x49, 0x6d, 0x8b, 0xd1, 0x25,
    0x72, 0xf8, 0xf6, 0x64, 0x86, 0x68, 0x98, 0x16, 0xd4, 0xa4, 0x5c, 0xcc, 0x5d, 0x65, 0xb6, 0x92,
    0x6c, 0x70, 0x48, 0x50, 0xfd, 0xed, 0xb9, 0xda, 0x5e, 0x15, 0x46, 0x57, 0xa7, 0x8d, 0x9d, 0x84,
    0x90, 0xd8, 0xab, 0x00, 0x8c, 0xbc, 0xd3, 0x0a, 0xf7, 0xe4, 0x58, 0x05, 0xb8, 0xb3, 0x45, 0x06,
    0xd0, 0x2c, 0x1e, 0x8f, 0xca, 0x3f, 0x0f, 0x02, 0xc1, 0xaf, 0xbd, 0x03, 0x01, 0x13, 0x8a, 0x6b,
    0x3a, 0x91, 0x11, 0x41, 0x4f, 0x67, 0xdc, 0xea, 0x97, 0xf2, 0xcf, 0xce, 0xf0, 0xb4, 0xe6, 0x73,
    0x96, 0xac, 0x74, 0x22, 0xe7, 0xad, 0x35, 0x85, 0xe2, 0xf9, 0x37, 0xe8, 0x1c, 0x75, 0xdf, 0x6e,
    0x47, 0xf1, 0x1a, 0x71, 0x1d, 0x29, 0xc5, 0x89, 0x6f, 0xb7, 0x62, 0x0e, 0xaa, 0x18, 0xbe, 0x1b,
    0xfc, 0x56, 0x3e, 0x4b, 0xc6, 0xd2, 0x79, 0x20, 0x9a, 0xdb, 0xc0, 0xfe, 0x78, 0xcd, 0x5a, 0xf4,
    0x1f, 0xdd, 0xa8, 0x33, 0x88, 0x07, 0xc7, 0x31, 0xb1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xec, 0x5f,
    0x60, 0x51, 0x7f, 0xa9, 0x19, 0xb5, 0x4a, 0x0d, 0x2d, 0xe5, 0x7a, 0x9f, 0x93, 0xc9, 0x9c, 0xef,
    0xa0, 0xe0, 0x3b, 0x4d, 0xae, 0x2a, 0xf5, 0xb0, 0xc8, 0xeb, 0xbb, 0x3c, 0x83, 0x53, 0x99, 0x61,
    0x17, 0x2b, 0x04, 0x7e, 0xba, 0x77, 0xd6, 0x26, 0xe1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0c, 0x7d
};

/*
 * ROUND CONSTANTS (RCON)
 * Used in key expansion algorithm
 */
static const uint8_t RCON[11] = {
    0x00, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36
};

/*
 * GALOIS FIELD MULTIPLICATION
 * 
 * AES uses arithmetic in GF(2^8) - Galois Field with 256 elements.
 * This is used in the MixColumns transformation.
 */
static uint8_t gf_multiply(uint8_t a, uint8_t b) {
    uint8_t result = 0;
    uint8_t high_bit;
    
    for (int i = 0; i < 8; i++) {
        if (b & 1) {
            result ^= a;
        }
        
        high_bit = a & 0x80;
        a <<= 1;
        
        if (high_bit) {
            a ^= 0x1b;  // AES irreducible polynomial: x^8 + x^4 + x^3 + x + 1
        }
        
        b >>= 1;
    }
    
    return result;
}

/*
 * AES INITIALIZATION
 * 
 * Sets up the AES context with the encryption key and generates
 * all round keys needed for encryption/decryption.
 */
int aes_init(aes_context_t* ctx, const uint8_t* key, int key_size) {
    printf("🔐 Initializing AES with %d-bit key\n", key_size * 8);
    
    // Validate key size
    if (key_size != 16 && key_size != 24 && key_size != 32) {
        printf("❌ Invalid key size: %d (must be 16, 24, or 32 bytes)\n", key_size);
        return -1;
    }
    
    // Set context parameters
    ctx->key_size = key_size;
    memcpy(ctx->key, key, key_size);
    
    // Determine number of rounds based on key size
    switch (key_size) {
        case 16: ctx->rounds = AES_ROUNDS_128; break;  // AES-128: 10 rounds
        case 24: ctx->rounds = AES_ROUNDS_192; break;  // AES-192: 12 rounds
        case 32: ctx->rounds = AES_ROUNDS_256; break;  // AES-256: 14 rounds
    }
    
    printf("   Key size: %d bytes (%d bits)\n", key_size, key_size * 8);
    printf("   Rounds: %d\n", ctx->rounds);
    
    // Generate round keys
    aes_key_expansion(ctx, key, key_size);
    
    printf("✅ AES initialization complete\n");
    return 0;
}

/*
 * AES KEY EXPANSION
 * 
 * Generates round keys from the main encryption key.
 * This is one of the most complex parts of AES.
 */
void aes_key_expansion(aes_context_t* ctx, const uint8_t* key, int key_size) {
    printf("🔑 Expanding %d-byte key into %d round keys\n", key_size, ctx->rounds + 1);
    
    int key_words = key_size / 4;           // Number of 32-bit words in key
    int total_words = 4 * (ctx->rounds + 1); // Total words needed
    uint32_t* w = (uint32_t*)ctx->round_keys; // Treat as array of 32-bit words
    
    // Copy original key as first round key(s)
    for (int i = 0; i < key_words; i++) {
        w[i] = ((uint32_t)key[4*i] << 24) |
               ((uint32_t)key[4*i+1] << 16) |
               ((uint32_t)key[4*i+2] << 8) |
               ((uint32_t)key[4*i+3]);
    }
    
    // Generate remaining round keys
    for (int i = key_words; i < total_words; i++) {
        uint32_t temp = w[i-1];
        
        if (i % key_words == 0) {
            // RotWord: rotate left by 8 bits
            temp = (temp << 8) | (temp >> 24);
            
            // SubWord: apply S-box to each byte
            temp = ((uint32_t)S_BOX[(temp >> 24) & 0xff] << 24) |
                   ((uint32_t)S_BOX[(temp >> 16) & 0xff] << 16) |
                   ((uint32_t)S_BOX[(temp >> 8) & 0xff] << 8) |
                   ((uint32_t)S_BOX[temp & 0xff]);
            
            // XOR with round constant
            temp ^= ((uint32_t)RCON[i / key_words] << 24);
        }
        else if (key_words > 6 && i % key_words == 4) {
            // Additional SubWord for AES-256
            temp = ((uint32_t)S_BOX[(temp >> 24) & 0xff] << 24) |
                   ((uint32_t)S_BOX[(temp >> 16) & 0xff] << 16) |
                   ((uint32_t)S_BOX[(temp >> 8) & 0xff] << 8) |
                   ((uint32_t)S_BOX[temp & 0xff]);
        }
        
        w[i] = w[i - key_words] ^ temp;
    }
    
    printf("✅ Key expansion complete\n");
}

/*
 * AES SUBBYTES TRANSFORMATION
 * 
 * Applies the S-box substitution to each byte of the state.
 * This provides non-linearity to resist linear cryptanalysis.
 */
void aes_sub_bytes(uint8_t state[16]) {
    for (int i = 0; i < 16; i++) {
        state[i] = S_BOX[state[i]];
    }
}

/*
 * AES INVERSE SUBBYTES (for decryption)
 */
void aes_inv_sub_bytes(uint8_t state[16]) {
    for (int i = 0; i < 16; i++) {
        state[i] = INV_S_BOX[state[i]];
    }
}

/*
 * AES SHIFTROWS TRANSFORMATION
 * 
 * Shifts the rows of the state matrix:
 * - Row 0: no shift
 * - Row 1: shift left by 1
 * - Row 2: shift left by 2  
 * - Row 3: shift left by 3
 * 
 * State matrix layout (column-major):
 * [ 0  4  8 12]
 * [ 1  5  9 13]
 * [ 2  6 10 14]
 * [ 3  7 11 15]
 */
void aes_shift_rows(uint8_t state[16]) {
    uint8_t temp;
    
    // Row 1: shift left by 1
    temp = state[1];
    state[1] = state[5];
    state[5] = state[9];
    state[9] = state[13];
    state[13] = temp;
    
    // Row 2: shift left by 2
    temp = state[2];
    state[2] = state[10];
    state[10] = temp;
    temp = state[6];
    state[6] = state[14];
    state[14] = temp;
    
    // Row 3: shift left by 3 (or right by 1)
    temp = state[3];
    state[3] = state[15];
    state[15] = state[11];
    state[11] = state[7];
    state[7] = temp;
}

/*
 * AES INVERSE SHIFTROWS (for decryption)
 */
void aes_inv_shift_rows(uint8_t state[16]) {
    uint8_t temp;
    
    // Row 1: shift right by 1
    temp = state[13];
    state[13] = state[9];
    state[9] = state[5];
    state[5] = state[1];
    state[1] = temp;
    
    // Row 2: shift right by 2
    temp = state[2];
    state[2] = state[10];
    state[10] = temp;
    temp = state[6];
    state[6] = state[14];
    state[14] = temp;
    
    // Row 3: shift right by 3 (or left by 1)
    temp = state[7];
    state[7] = state[11];
    state[11] = state[15];
    state[15] = state[3];
    state[3] = temp;
}

/*
 * AES MIXCOLUMNS TRANSFORMATION
 * 
 * Mixes the columns using matrix multiplication in GF(2^8).
 * Each column is multiplied by the fixed matrix:
 * [2 3 1 1]
 * [1 2 3 1]
 * [1 1 2 3]
 * [3 1 1 2]
 */
void aes_mix_columns(uint8_t state[16]) {
    for (int col = 0; col < 4; col++) {
        uint8_t* column = &state[col * 4];
        uint8_t temp[4];
        
        temp[0] = gf_multiply(2, column[0]) ^ gf_multiply(3, column[1]) ^ column[2] ^ column[3];
        temp[1] = column[0] ^ gf_multiply(2, column[1]) ^ gf_multiply(3, column[2]) ^ column[3];
        temp[2] = column[0] ^ column[1] ^ gf_multiply(2, column[2]) ^ gf_multiply(3, column[3]);
        temp[3] = gf_multiply(3, column[0]) ^ column[1] ^ column[2] ^ gf_multiply(2, column[3]);
        
        memcpy(column, temp, 4);
    }
}

/*
 * AES INVERSE MIXCOLUMNS (for decryption)
 */
void aes_inv_mix_columns(uint8_t state[16]) {
    for (int col = 0; col < 4; col++) {
        uint8_t* column = &state[col * 4];
        uint8_t temp[4];
        
        temp[0] = gf_multiply(0x0e, column[0]) ^ gf_multiply(0x0b, column[1]) ^ 
                  gf_multiply(0x0d, column[2]) ^ gf_multiply(0x09, column[3]);
        temp[1] = gf_multiply(0x09, column[0]) ^ gf_multiply(0x0e, column[1]) ^ 
                  gf_multiply(0x0b, column[2]) ^ gf_multiply(0x0d, column[3]);
        temp[2] = gf_multiply(0x0d, column[0]) ^ gf_multiply(0x09, column[1]) ^ 
                  gf_multiply(0x0e, column[2]) ^ gf_multiply(0x0b, column[3]);
        temp[3] = gf_multiply(0x0b, column[0]) ^ gf_multiply(0x0d, column[1]) ^ 
                  gf_multiply(0x09, column[2]) ^ gf_multiply(0x0e, column[3]);
        
        memcpy(column, temp, 4);
    }
}

/*
 * AES ADD ROUND KEY
 * 
 * XORs the state with the round key.
 * This is the only step that uses the key material.
 */
void aes_add_round_key(uint8_t state[16], const uint8_t round_key[16]) {
    for (int i = 0; i < 16; i++) {
        state[i] ^= round_key[i];
    }
}

/*
 * AES BLOCK ENCRYPTION
 * 
 * Encrypts a single 16-byte block using the AES algorithm.
 */
void aes_encrypt_block(const aes_context_t* ctx, const uint8_t* plaintext, uint8_t* ciphertext) {
    // Copy plaintext to state (we'll modify it in place)
    memcpy(ciphertext, plaintext, AES_BLOCK_SIZE);
    
    // Initial round: AddRoundKey
    aes_add_round_key(ciphertext, ctx->round_keys[0]);
    
    // Main rounds
    for (int round = 1; round < ctx->rounds; round++) {
        aes_sub_bytes(ciphertext);
        aes_shift_rows(ciphertext);
        aes_mix_columns(ciphertext);
        aes_add_round_key(ciphertext, ctx->round_keys[round]);
    }
    
    // Final round (no MixColumns)
    aes_sub_bytes(ciphertext);
    aes_shift_rows(ciphertext);
    aes_add_round_key(ciphertext, ctx->round_keys[ctx->rounds]);
}

/*
 * AES BLOCK DECRYPTION
 * 
 * Decrypts a single 16-byte block using the AES algorithm.
 * This reverses the encryption process.
 */
void aes_decrypt_block(const aes_context_t* ctx, const uint8_t* ciphertext, uint8_t* plaintext) {
    // Copy ciphertext to state
    memcpy(plaintext, ciphertext, AES_BLOCK_SIZE);
    
    // Reverse final round
    aes_add_round_key(plaintext, ctx->round_keys[ctx->rounds]);
    aes_inv_shift_rows(plaintext);
    aes_inv_sub_bytes(plaintext);
    
    // Reverse main rounds
    for (int round = ctx->rounds - 1; round > 0; round--) {
        aes_add_round_key(plaintext, ctx->round_keys[round]);
        aes_inv_mix_columns(plaintext);
        aes_inv_shift_rows(plaintext);
        aes_inv_sub_bytes(plaintext);
    }
    
    // Reverse initial round
    aes_add_round_key(plaintext, ctx->round_keys[0]);
}

/*
 * AES ECB MODE ENCRYPTION
 * 
 * ECB (Electronic Codebook) mode encrypts each block independently.
 * WARNING: ECB is not secure for most applications! It's used here
 * for educational purposes only.
 */
void aes_encrypt_ecb(const aes_context_t* ctx, const uint8_t* input, uint8_t* output, size_t length) {
    printf("🔒 Encrypting %zu bytes using AES-ECB\n", length);
    
    if (length % AES_BLOCK_SIZE != 0) {
        printf("❌ Input length must be multiple of %d bytes\n", AES_BLOCK_SIZE);
        return;
    }
    
    size_t blocks = length / AES_BLOCK_SIZE;
    printf("   Processing %zu blocks\n", blocks);
    
    for (size_t i = 0; i < blocks; i++) {
        aes_encrypt_block(ctx, input + i * AES_BLOCK_SIZE, output + i * AES_BLOCK_SIZE);
    }
    
    printf("✅ Encryption complete\n");
}

/*
 * AES ECB MODE DECRYPTION
 */
void aes_decrypt_ecb(const aes_context_t* ctx, const uint8_t* input, uint8_t* output, size_t length) {
    printf("🔓 Decrypting %zu bytes using AES-ECB\n", length);
    
    if (length % AES_BLOCK_SIZE != 0) {
        printf("❌ Input length must be multiple of %d bytes\n", AES_BLOCK_SIZE);
        return;
    }
    
    size_t blocks = length / AES_BLOCK_SIZE;
    printf("   Processing %zu blocks\n", blocks);
    
    for (size_t i = 0; i < blocks; i++) {
        aes_decrypt_block(ctx, input + i * AES_BLOCK_SIZE, output + i * AES_BLOCK_SIZE);
    }
    
    printf("✅ Decryption complete\n");
}