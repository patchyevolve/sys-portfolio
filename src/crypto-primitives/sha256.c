/*
 * SHA-256 (Secure Hash Algorithm 256-bit) Implementation
 * 
 * SHA-256 is a cryptographic hash function that produces a 256-bit digest.
 * It's part of the SHA-2 family and is widely used in cryptographic applications.
 * 
 * ALGORITHM OVERVIEW:
 * 1. Preprocessing: Pad message and append length
 * 2. Initialize hash values (8 x 32-bit words)
 * 3. Process message in 512-bit chunks
 * 4. For each chunk, perform 64 rounds of operations
 * 5. Add chunk's hash to result
 */

#include "crypto.h"
#include <stdio.h>
#include <string.h>

/*
 * SHA-256 CONSTANTS
 * 
 * These are the first 32 bits of the fractional parts of the cube roots
 * of the first 64 prime numbers. They provide "nothing up my sleeve" numbers.
 */
static const uint32_t K[64] = {
    0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
    0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
    0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
    0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
    0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
    0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
    0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
    0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
};

/*
 * INITIAL HASH VALUES
 * 
 * These are the first 32 bits of the fractional parts of the square roots
 * of the first 8 prime numbers.
 */
static const uint32_t H0[8] = {
    0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a,
    0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19
};

/*
 * UTILITY FUNCTIONS FOR BIT OPERATIONS
 */

// Right rotate a 32-bit value by n bits
static uint32_t rotr(uint32_t value, int n) {
    return (value >> n) | (value << (32 - n));
}

// SHA-256 choice function
static uint32_t ch(uint32_t x, uint32_t y, uint32_t z) {
    return (x & y) ^ (~x & z);
}

// SHA-256 majority function  
static uint32_t maj(uint32_t x, uint32_t y, uint32_t z) {
    return (x & y) ^ (x & z) ^ (y & z);
}

// SHA-256 sigma functions
static uint32_t sigma0(uint32_t x) {
    return rotr(x, 2) ^ rotr(x, 13) ^ rotr(x, 22);
}

static uint32_t sigma1(uint32_t x) {
    return rotr(x, 6) ^ rotr(x, 11) ^ rotr(x, 25);
}

static uint32_t gamma0(uint32_t x) {
    return rotr(x, 7) ^ rotr(x, 18) ^ (x >> 3);
}

static uint32_t gamma1(uint32_t x) {
    return rotr(x, 17) ^ rotr(x, 19) ^ (x >> 10);
}

/*
 * SHA-256 INITIALIZATION
 */
void sha256_init(sha256_context_t* ctx) {
    printf("🔗 Initializing SHA-256 context\n");
    
    // Initialize hash state with standard values
    memcpy(ctx->state, H0, sizeof(H0));
    
    // Initialize counters
    ctx->bit_count = 0;
    ctx->buffer_len = 0;
    
    printf("✅ SHA-256 context initialized\n");
}

/*
 * PROCESS A SINGLE 512-BIT BLOCK
 * 
 * This is the core of the SHA-256 algorithm.
 * It processes one 512-bit (64-byte) block of data.
 */
static void sha256_process_block(sha256_context_t* ctx, const uint8_t* block) {
    uint32_t w[64];  // Message schedule
    uint32_t a, b, c, d, e, f, g, h;  // Working variables
    uint32_t t1, t2;
    
    // Prepare message schedule (first 16 words are from input)
    for (int i = 0; i < 16; i++) {
        w[i] = ((uint32_t)block[i*4] << 24) |
               ((uint32_t)block[i*4+1] << 16) |
               ((uint32_t)block[i*4+2] << 8) |
               ((uint32_t)block[i*4+3]);
    }
    
    // Extend message schedule (words 16-63)
    for (int i = 16; i < 64; i++) {
        w[i] = gamma1(w[i-2]) + w[i-7] + gamma0(w[i-15]) + w[i-16];
    }
    
    // Initialize working variables
    a = ctx->state[0];
    b = ctx->state[1];
    c = ctx->state[2];
    d = ctx->state[3];
    e = ctx->state[4];
    f = ctx->state[5];
    g = ctx->state[6];
    h = ctx->state[7];
    
    // Main loop (64 rounds)
    for (int i = 0; i < 64; i++) {
        t1 = h + sigma1(e) + ch(e, f, g) + K[i] + w[i];
        t2 = sigma0(a) + maj(a, b, c);
        
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;
    }
    
    // Add this chunk's hash to result
    ctx->state[0] += a;
    ctx->state[1] += b;
    ctx->state[2] += c;
    ctx->state[3] += d;
    ctx->state[4] += e;
    ctx->state[5] += f;
    ctx->state[6] += g;
    ctx->state[7] += h;
}

/*
 * SHA-256 UPDATE
 * 
 * Process input data. Can be called multiple times.
 */
void sha256_update(sha256_context_t* ctx, const uint8_t* data, size_t length) {
    printf("📝 Processing %zu bytes through SHA-256\n", length);
    
    ctx->bit_count += length * 8;
    
    // Process any remaining data from previous update
    if (ctx->buffer_len > 0) {
        size_t remaining = SHA256_BLOCK_SIZE - ctx->buffer_len;
        size_t to_copy = (length < remaining) ? length : remaining;
        
        memcpy(ctx->buffer + ctx->buffer_len, data, to_copy);
        ctx->buffer_len += to_copy;
        data += to_copy;
        length -= to_copy;
        
        // If we have a complete block, process it
        if (ctx->buffer_len == SHA256_BLOCK_SIZE) {
            sha256_process_block(ctx, ctx->buffer);
            ctx->buffer_len = 0;
        }
    }
    
    // Process complete blocks from input
    while (length >= SHA256_BLOCK_SIZE) {
        sha256_process_block(ctx, data);
        data += SHA256_BLOCK_SIZE;
        length -= SHA256_BLOCK_SIZE;
    }
    
    // Store remaining data in buffer
    if (length > 0) {
        memcpy(ctx->buffer + ctx->buffer_len, data, length);
        ctx->buffer_len += length;
    }
}

/*
 * SHA-256 FINALIZATION
 * 
 * Applies padding and produces the final hash digest.
 */
void sha256_final(sha256_context_t* ctx, uint8_t digest[SHA256_DIGEST_SIZE]) {
    printf("🏁 Finalizing SHA-256 hash\n");
    
    // Add padding bit (0x80)
    ctx->buffer[ctx->buffer_len++] = 0x80;
    
    // If not enough room for length, pad to block boundary and process
    if (ctx->buffer_len > 56) {
        while (ctx->buffer_len < SHA256_BLOCK_SIZE) {
            ctx->buffer[ctx->buffer_len++] = 0x00;
        }
        sha256_process_block(ctx, ctx->buffer);
        ctx->buffer_len = 0;
    }
    
    // Pad with zeros
    while (ctx->buffer_len < 56) {
        ctx->buffer[ctx->buffer_len++] = 0x00;
    }
    
    // Append length as 64-bit big-endian integer
    for (int i = 7; i >= 0; i--) {
        ctx->buffer[56 + (7-i)] = (ctx->bit_count >> (i * 8)) & 0xff;
    }
    
    // Process final block
    sha256_process_block(ctx, ctx->buffer);
    
    // Convert hash state to bytes (big-endian)
    for (int i = 0; i < 8; i++) {
        digest[i*4] = (ctx->state[i] >> 24) & 0xff;
        digest[i*4+1] = (ctx->state[i] >> 16) & 0xff;
        digest[i*4+2] = (ctx->state[i] >> 8) & 0xff;
        digest[i*4+3] = ctx->state[i] & 0xff;
    }
    
    printf("✅ SHA-256 hash computed\n");
}

/*
 * CONVENIENCE FUNCTION
 * 
 * Hash data in a single function call.
 */
void sha256_hash(const uint8_t* data, size_t length, uint8_t digest[SHA256_DIGEST_SIZE]) {
    sha256_context_t ctx;
    
    sha256_init(&ctx);
    sha256_update(&ctx, data, length);
    sha256_final(&ctx, digest);
}