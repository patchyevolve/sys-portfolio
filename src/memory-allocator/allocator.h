/*
 * Custom Memory Allocator - Header File
 * 
 * LEARNING OBJECTIVES:
 * - Understand how malloc() and free() work internally
 * - Learn about memory management strategies
 * - Explore linked list data structures
 * - Practice pointer manipulation in C
 * 
 * CONCEPTS COVERED:
 * - Memory alignment
 * - Block headers and metadata
 * - First-fit allocation strategy
 * - Memory fragmentation
 * - Coalescing free blocks
 */

#ifndef ALLOCATOR_H
#define ALLOCATOR_H

#include <stddef.h>  // For size_t
#include <stdint.h>  // For fixed-width integer types
#include <stdbool.h> // For bool type

/*
 * MEMORY BLOCK STRUCTURE
 * 
 * Each allocated block has a header that contains metadata:
 * - size: Size of the data portion (not including header)
 * - next: Pointer to next block in the free list
 * - free: Flag indicating if block is free (1) or allocated (0)
 * 
 * Memory layout:
 * [HEADER][DATA][HEADER][DATA]...
 */
typedef struct block {
    size_t size;        // Size of data portion in bytes
    struct block* next; // Next block in free list (only used when free)
    int free;          // 1 if free, 0 if allocated
} block_t;

/*
 * CONSTANTS AND CONFIGURATION
 */
#define BLOCK_SIZE sizeof(block_t)  // Size of block header
#define ALIGNMENT 8                 // Memory alignment (8-byte boundary)
#define MIN_BLOCK_SIZE 16          // Minimum allocatable block size

/*
 * GLOBAL VARIABLES
 * In a real implementation, these would be static/private
 */
extern block_t* heap_start;  // Pointer to first block in heap
extern size_t total_heap_size; // Total size of heap
extern size_t allocated_bytes; // Currently allocated bytes

/*
 * CORE ALLOCATOR FUNCTIONS
 */

/**
 * Initialize the memory allocator with a given heap size
 * @param heap_size: Size of heap to create in bytes
 * @return: 0 on success, -1 on failure
 */
int allocator_init(size_t heap_size);

/**
 * Allocate memory block of given size
 * @param size: Number of bytes to allocate
 * @return: Pointer to allocated memory, or NULL if allocation fails
 */
void* my_malloc(size_t size);

/**
 * Free previously allocated memory block
 * @param ptr: Pointer to memory block to free
 */
void my_free(void* ptr);

/**
 * Reallocate memory block to new size
 * @param ptr: Pointer to existing block (or NULL)
 * @param size: New size in bytes
 * @return: Pointer to reallocated memory, or NULL on failure
 */
void* my_realloc(void* ptr, size_t size);

/*
 * UTILITY FUNCTIONS
 */

/**
 * Find a free block that can fit the requested size
 * @param size: Required size in bytes
 * @return: Pointer to suitable block, or NULL if none found
 */
block_t* find_free_block(size_t size);

/**
 * Split a block if it's larger than needed
 * @param block: Block to potentially split
 * @param size: Required size
 */
void split_block(block_t* block, size_t size);

/**
 * Coalesce adjacent free blocks to reduce fragmentation
 * @param block: Starting block for coalescing
 */
void coalesce_blocks(block_t* block);

/**
 * Align size to proper boundary (8-byte alignment)
 * @param size: Size to align
 * @return: Aligned size
 */
size_t align_size(size_t size);

/*
 * DEBUG AND STATISTICS FUNCTIONS
 */

/**
 * Print heap statistics for debugging
 */
void print_heap_stats(void);

/**
 * Print entire heap layout for debugging
 */
void print_heap_layout(void);

/**
 * Validate heap integrity (check for corruption)
 * @return: true if heap is valid, false if corrupted
 */
bool validate_heap(void);

/**
 * Get fragmentation percentage
 * @return: Fragmentation as percentage (0-100)
 */
int get_fragmentation_percent(void);

#endif // ALLOCATOR_H