/*
 * Memory Allocator Test Program
 * 
 * This program demonstrates and tests our custom memory allocator.
 * It's designed to be educational and show how the allocator behaves
 * under different scenarios.
 */

#include "allocator.h"
#include <stdio.h>
#include <string.h>
#include <assert.h>

/*
 * TEST SCENARIOS
 * 
 * We'll test various allocation patterns to understand
 * how our allocator behaves and performs.
 */

void test_basic_allocation(void) {
    printf("\n🧪 TEST 1: Basic Allocation and Deallocation\n");
    printf("==============================================\n");
    
    // Test simple allocation
    void* ptr1 = my_malloc(64);
    assert(ptr1 != NULL);
    strcpy((char*)ptr1, "Hello, World!");
    printf("Stored string: '%s'\n", (char*)ptr1);
    
    void* ptr2 = my_malloc(128);
    assert(ptr2 != NULL);
    
    void* ptr3 = my_malloc(32);
    assert(ptr3 != NULL);
    
    print_heap_layout();
    print_heap_stats();
    
    // Free middle block to create fragmentation
    my_free(ptr2);
    printf("\nAfter freeing middle block:\n");
    print_heap_layout();
    
    // Free remaining blocks
    my_free(ptr1);
    my_free(ptr3);
    
    printf("\nAfter freeing all blocks:\n");
    print_heap_layout();
    print_heap_stats();
}

void test_fragmentation(void) {
    printf("\n🧪 TEST 2: Fragmentation and Coalescing\n");
    printf("========================================\n");
    
    // Allocate several blocks
    void* ptrs[5];
    for (int i = 0; i < 5; i++) {
        ptrs[i] = my_malloc(100);
        assert(ptrs[i] != NULL);
        printf("Allocated block %d at %p\n", i, ptrs[i]);
    }
    
    print_heap_layout();
    
    // Free every other block to create fragmentation
    printf("\nFreeing blocks 1 and 3 to create fragmentation:\n");
    my_free(ptrs[1]);
    my_free(ptrs[3]);
    
    print_heap_layout();
    printf("Fragmentation: %d%%\n", get_fragmentation_percent());
    
    // Try to allocate a large block (should fail due to fragmentation)
    printf("\nTrying to allocate 250 bytes (should fail):\n");
    void* large_ptr = my_malloc(250);
    if (large_ptr == NULL) {
        printf("❌ Allocation failed due to fragmentation\n");
    }
    
    // Free adjacent blocks to test coalescing
    printf("\nFreeing adjacent blocks to test coalescing:\n");
    my_free(ptrs[0]);  // This should coalesce with ptrs[1]
    my_free(ptrs[2]);  // This should coalesce with ptrs[3]
    
    print_heap_layout();
    printf("Fragmentation after coalescing: %d%%\n", get_fragmentation_percent());
    
    // Now the large allocation should succeed
    printf("\nTrying to allocate 250 bytes again:\n");
    large_ptr = my_malloc(250);
    if (large_ptr != NULL) {
        printf("✅ Large allocation succeeded after coalescing\n");
        my_free(large_ptr);
    }
    
    // Clean up
    my_free(ptrs[4]);
}

void test_reallocation(void) {
    printf("\n🧪 TEST 3: Reallocation\n");
    printf("========================\n");
    
    // Start with small allocation
    void* ptr = my_malloc(50);
    assert(ptr != NULL);
    strcpy((char*)ptr, "Initial data");
    printf("Initial allocation: '%s' at %p\n", (char*)ptr, ptr);
    
    print_heap_layout();
    
    // Grow the allocation
    ptr = my_realloc(ptr, 150);
    assert(ptr != NULL);
    printf("After growing to 150 bytes: '%s' at %p\n", (char*)ptr, ptr);
    
    print_heap_layout();
    
    // Shrink the allocation
    ptr = my_realloc(ptr, 25);
    assert(ptr != NULL);
    printf("After shrinking to 25 bytes: '%s' at %p\n", (char*)ptr, ptr);
    
    print_heap_layout();
    
    // Clean up
    my_free(ptr);
}

void test_stress_allocation(void) {
    printf("\n🧪 TEST 4: Stress Test\n");
    printf("=======================\n");
    
    const int NUM_ALLOCS = 20;
    void* ptrs[NUM_ALLOCS];
    
    // Allocate many small blocks
    printf("Allocating %d small blocks...\n", NUM_ALLOCS);
    for (int i = 0; i < NUM_ALLOCS; i++) {
        size_t size = 16 + (i * 8);  // Varying sizes
        ptrs[i] = my_malloc(size);
        if (ptrs[i] == NULL) {
            printf("❌ Allocation %d failed (size %zu)\n", i, size);
            break;
        }
    }
    
    print_heap_stats();
    
    // Free random blocks
    printf("\nFreeing random blocks...\n");
    for (int i = 0; i < NUM_ALLOCS; i += 3) {
        if (ptrs[i] != NULL) {
            my_free(ptrs[i]);
            ptrs[i] = NULL;
        }
    }
    
    print_heap_stats();
    printf("Fragmentation: %d%%\n", get_fragmentation_percent());
    
    // Clean up remaining blocks
    printf("\nCleaning up remaining blocks...\n");
    for (int i = 0; i < NUM_ALLOCS; i++) {
        if (ptrs[i] != NULL) {
            my_free(ptrs[i]);
        }
    }
    
    print_heap_stats();
}

void test_edge_cases(void) {
    printf("\n🧪 TEST 5: Edge Cases\n");
    printf("=====================\n");
    
    // Test zero allocation
    printf("Testing zero-byte allocation:\n");
    void* ptr = my_malloc(0);
    printf("my_malloc(0) returned: %p\n", ptr);
    
    // Test NULL free
    printf("\nTesting free(NULL):\n");
    my_free(NULL);
    
    // Test double free (should be detected)
    printf("\nTesting double free:\n");
    ptr = my_malloc(64);
    my_free(ptr);
    my_free(ptr);  // Should print warning
    
    // Test very large allocation
    printf("\nTesting very large allocation:\n");
    void* large_ptr = my_malloc(1000000);  // 1MB
    if (large_ptr == NULL) {
        printf("❌ Large allocation failed (expected)\n");
    } else {
        printf("✅ Large allocation succeeded\n");
        my_free(large_ptr);
    }
}

/*
 * MAIN FUNCTION
 * 
 * Runs all our tests to demonstrate the allocator's behavior.
 */
int main(void) {
    printf("🎯 CUSTOM MEMORY ALLOCATOR DEMONSTRATION\n");
    printf("=========================================\n");
    
    // Initialize our allocator with 4KB heap
    if (allocator_init(4096) != 0) {
        printf("❌ Failed to initialize allocator\n");
        return 1;
    }
    
    // Run test suite
    test_basic_allocation();
    test_fragmentation();
    test_reallocation();
    test_stress_allocation();
    test_edge_cases();
    
    // Final heap validation
    printf("\n🔍 FINAL HEAP VALIDATION\n");
    printf("========================\n");
    validate_heap();
    print_heap_stats();
    
    printf("\n🎉 All tests completed!\n");
    printf("\nKEY LEARNINGS:\n");
    printf("- Memory allocation requires careful bookkeeping\n");
    printf("- Fragmentation is a real problem in dynamic allocation\n");
    printf("- Coalescing helps reduce external fragmentation\n");
    printf("- Alignment is important for performance\n");
    printf("- Error handling is crucial for robust allocators\n");
    
    return 0;
}

/*
 * COMPILATION INSTRUCTIONS:
 * 
 * To compile and run this program:
 * 
 * gcc -o test_allocator allocator.c test_allocator.c -std=c99 -Wall -Wextra
 * ./test_allocator
 * 
 * For debugging with extra information:
 * gcc -g -DDEBUG -o test_allocator allocator.c test_allocator.c -std=c99 -Wall -Wextra
 * 
 * To run with memory debugging (if available):
 * valgrind ./test_allocator
 */