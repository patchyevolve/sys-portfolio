/*
 * Custom Memory Allocator - Implementation
 * 
 * This is a simplified version of malloc/free that demonstrates
 * the core concepts of dynamic memory management.
 * 
 * ALGORITHM: First-Fit with Coalescing
 * - Maintains a linked list of free blocks
 * - Uses first block that fits the requested size
 * - Coalesces adjacent free blocks to reduce fragmentation
 */

#include "allocator.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>    // For sbrk() system call
#include <assert.h>

/*
 * GLOBAL STATE VARIABLES
 * In production code, these would be static to this file
 */
block_t* heap_start = NULL;    // First block in our heap
size_t total_heap_size = 0;    // Total heap size
size_t allocated_bytes = 0;    // Currently allocated bytes

/*
 * STEP 1: INITIALIZATION
 * 
 * Before we can allocate memory, we need to set up our heap.
 * This involves getting memory from the OS and initializing our data structures.
 */
int allocator_init(size_t heap_size) {
    printf("🚀 Initializing custom allocator with %zu bytes\n", heap_size);
    
    // Align heap size to page boundary for efficiency
    heap_size = (heap_size + 4095) & ~4095;  // Round up to 4KB boundary
    
    // Get memory from OS using sbrk() system call
    // sbrk() extends the program's data segment
    void* heap_memory = sbrk(heap_size);
    if (heap_memory == (void*)-1) {
        printf("❌ Failed to allocate heap memory\n");
        return -1;
    }
    
    // Initialize the first (and only) free block
    heap_start = (block_t*)heap_memory;
    heap_start->size = heap_size - BLOCK_SIZE;  // Subtract header size
    heap_start->next = NULL;                    // No next block yet
    heap_start->free = 1;                       // Mark as free
    
    total_heap_size = heap_size;
    allocated_bytes = 0;
    
    printf("✅ Heap initialized: %p to %p (%zu bytes)\n", 
           heap_memory, (char*)heap_memory + heap_size, heap_size);
    
    return 0;
}

/*
 * STEP 2: SIZE ALIGNMENT
 * 
 * Memory alignment is crucial for performance and correctness.
 * Most architectures require data to be aligned to specific boundaries.
 */
size_t align_size(size_t size) {
    // Align to 8-byte boundary for optimal performance
    // This ensures pointers and 64-bit values are properly aligned
    return (size + ALIGNMENT - 1) & ~(ALIGNMENT - 1);
}

/*
 * STEP 3: FINDING FREE BLOCKS
 * 
 * The first-fit algorithm searches for the first block that's
 * large enough to satisfy the allocation request.
 */
block_t* find_free_block(size_t size) {
    block_t* current = heap_start;
    
    printf("🔍 Searching for block of size %zu bytes\n", size);
    
    // Walk through the linked list of blocks
    while (current != NULL) {
        printf("   Block at %p: size=%zu, free=%d\n", 
               current, current->size, current->free);
        
        // Check if this block is free and large enough
        if (current->free && current->size >= size) {
            printf("✅ Found suitable block at %p\n", current);
            return current;
        }
        
        current = current->next;
    }
    
    printf("❌ No suitable block found\n");
    return NULL;  // No suitable block found
}

/*
 * STEP 4: BLOCK SPLITTING
 * 
 * If a free block is much larger than needed, we split it
 * to avoid internal fragmentation.
 */
void split_block(block_t* block, size_t size) {
    // Only split if the remaining space is large enough for a new block
    size_t remaining = block->size - size;
    
    if (remaining > BLOCK_SIZE + MIN_BLOCK_SIZE) {
        printf("✂️  Splitting block: %zu bytes -> %zu + %zu\n", 
               block->size, size, remaining - BLOCK_SIZE);
        
        // Create new block header after the allocated space
        block_t* new_block = (block_t*)((char*)block + BLOCK_SIZE + size);
        new_block->size = remaining - BLOCK_SIZE;
        new_block->free = 1;
        new_block->next = block->next;
        
        // Update original block
        block->size = size;
        block->next = new_block;
    }
}

/*
 * STEP 5: MAIN ALLOCATION FUNCTION
 * 
 * This is our version of malloc(). It finds a suitable block,
 * splits it if necessary, and returns a pointer to the data area.
 */
void* my_malloc(size_t size) {
    printf("\n📦 my_malloc(%zu bytes)\n", size);
    
    // Validate input
    if (size == 0) {
        printf("⚠️  Warning: Allocating 0 bytes\n");
        return NULL;
    }
    
    // Align size to proper boundary
    size = align_size(size);
    printf("   Aligned size: %zu bytes\n", size);
    
    // Find a suitable free block
    block_t* block = find_free_block(size);
    if (block == NULL) {
        printf("❌ Out of memory!\n");
        return NULL;
    }
    
    // Split block if it's much larger than needed
    split_block(block, size);
    
    // Mark block as allocated
    block->free = 0;
    allocated_bytes += block->size;
    
    // Return pointer to data area (after the header)
    void* data_ptr = (char*)block + BLOCK_SIZE;
    printf("✅ Allocated %zu bytes at %p\n", block->size, data_ptr);
    
    return data_ptr;
}

/*
 * STEP 6: BLOCK COALESCING
 * 
 * When blocks are freed, we try to merge adjacent free blocks
 * to reduce external fragmentation.
 */
void coalesce_blocks(block_t* block) {
    printf("🔗 Attempting to coalesce blocks starting at %p\n", block);
    
    // Coalesce with next block if it's free and adjacent
    while (block->next != NULL && block->next->free) {
        block_t* next_block = block->next;
        
        // Check if blocks are physically adjacent in memory
        char* block_end = (char*)block + BLOCK_SIZE + block->size;
        if (block_end == (char*)next_block) {
            printf("   Merging with next block: %zu + %zu = %zu bytes\n",
                   block->size, next_block->size + BLOCK_SIZE, 
                   block->size + next_block->size + BLOCK_SIZE);
            
            // Merge the blocks
            block->size += BLOCK_SIZE + next_block->size;
            block->next = next_block->next;
        } else {
            break;  // Blocks not adjacent
        }
    }
}

/*
 * STEP 7: FREEING MEMORY
 * 
 * This is our version of free(). It marks a block as free
 * and attempts to coalesce with adjacent free blocks.
 */
void my_free(void* ptr) {
    printf("\n🗑️  my_free(%p)\n", ptr);
    
    // Validate input
    if (ptr == NULL) {
        printf("⚠️  Warning: Attempting to free NULL pointer\n");
        return;
    }
    
    // Get block header (it's just before the data)
    block_t* block = (block_t*)((char*)ptr - BLOCK_SIZE);
    
    // Validate that this looks like a real block
    if (block < heap_start || 
        (char*)block >= (char*)heap_start + total_heap_size) {
        printf("❌ Error: Invalid pointer %p\n", ptr);
        return;
    }
    
    if (block->free) {
        printf("⚠️  Warning: Double free detected at %p\n", ptr);
        return;
    }
    
    printf("   Freeing block: size=%zu bytes\n", block->size);
    
    // Mark block as free
    block->free = 1;
    allocated_bytes -= block->size;
    
    // Try to coalesce with adjacent free blocks
    coalesce_blocks(block);
    
    printf("✅ Block freed successfully\n");
}

/*
 * STEP 8: REALLOCATION
 * 
 * Realloc is more complex - it may need to move data to a new location.
 */
void* my_realloc(void* ptr, size_t size) {
    printf("\n🔄 my_realloc(%p, %zu bytes)\n", ptr, size);
    
    // If ptr is NULL, behave like malloc
    if (ptr == NULL) {
        return my_malloc(size);
    }
    
    // If size is 0, behave like free
    if (size == 0) {
        my_free(ptr);
        return NULL;
    }
    
    // Get current block
    block_t* block = (block_t*)((char*)ptr - BLOCK_SIZE);
    size_t old_size = block->size;
    size = align_size(size);
    
    // If new size fits in current block, we're done
    if (size <= old_size) {
        printf("   Shrinking in place: %zu -> %zu bytes\n", old_size, size);
        // Could split block here if size difference is large
        return ptr;
    }
    
    // Need to allocate new block and copy data
    printf("   Need to relocate: %zu -> %zu bytes\n", old_size, size);
    void* new_ptr = my_malloc(size);
    if (new_ptr == NULL) {
        return NULL;  // Allocation failed
    }
    
    // Copy old data to new location
    memcpy(new_ptr, ptr, old_size);
    my_free(ptr);
    
    return new_ptr;
}

/*
 * STEP 9: DEBUGGING AND STATISTICS
 * 
 * These functions help us understand what's happening in our allocator.
 */
void print_heap_stats(void) {
    printf("\n📊 HEAP STATISTICS\n");
    printf("==================\n");
    printf("Total heap size:    %zu bytes\n", total_heap_size);
    printf("Allocated bytes:    %zu bytes\n", allocated_bytes);
    printf("Free bytes:         %zu bytes\n", total_heap_size - allocated_bytes);
    printf("Utilization:        %.1f%%\n", 
           (double)allocated_bytes / total_heap_size * 100);
    printf("Fragmentation:      %d%%\n", get_fragmentation_percent());
}

void print_heap_layout(void) {
    printf("\n🗺️  HEAP LAYOUT\n");
    printf("===============\n");
    
    block_t* current = heap_start;
    int block_num = 0;
    
    while (current != NULL) {
        printf("Block %d: %p | Size: %6zu | %s | Next: %p\n",
               block_num++, current, current->size,
               current->free ? "FREE " : "ALLOC", current->next);
        current = current->next;
    }
}

bool validate_heap(void) {
    printf("\n🔍 VALIDATING HEAP INTEGRITY\n");
    
    block_t* current = heap_start;
    size_t total_accounted = 0;
    
    while (current != NULL) {
        // Check if block is within heap bounds
        if ((char*)current < (char*)heap_start ||
            (char*)current >= (char*)heap_start + total_heap_size) {
            printf("❌ Block %p is outside heap bounds\n", current);
            return false;
        }
        
        total_accounted += current->size + BLOCK_SIZE;
        current = current->next;
    }
    
    printf("✅ Heap integrity check passed\n");
    return true;
}

int get_fragmentation_percent(void) {
    block_t* current = heap_start;
    int free_blocks = 0;
    size_t largest_free = 0;
    size_t total_free = 0;
    
    while (current != NULL) {
        if (current->free) {
            free_blocks++;
            total_free += current->size;
            if (current->size > largest_free) {
                largest_free = current->size;
            }
        }
        current = current->next;
    }
    
    if (total_free == 0) return 0;
    
    // Fragmentation = (1 - largest_free/total_free) * 100
    return (int)((1.0 - (double)largest_free / total_free) * 100);
}