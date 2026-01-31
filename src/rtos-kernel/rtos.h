/*
 * Real-Time Operating System (RTOS) Kernel - Header File
 * 
 * LEARNING OBJECTIVES:
 * - Understand how operating systems manage tasks
 * - Learn about context switching and scheduling
 * - Explore preemptive multitasking concepts
 * - Practice low-level system programming
 * - Understand memory management in embedded systems
 * 
 * CONCEPTS COVERED:
 * - Task Control Blocks (TCB)
 * - Round-robin scheduling with priorities
 * - Context switching (simulated)
 * - Task states and state transitions
 * - Stack management
 * - Interrupt handling concepts
 * - Synchronization primitives
 * 
 * TARGET: This is designed for educational purposes and ARM Cortex-M concepts
 */

#ifndef RTOS_H
#define RTOS_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/*
 * RTOS CONFIGURATION
 */
#define MAX_TASKS 8              // Maximum number of tasks
#define STACK_SIZE 1024          // Stack size per task (bytes)
#define TIME_SLICE_MS 10         // Time slice in milliseconds
#define PRIORITY_LEVELS 4        // Number of priority levels (0-3)

/*
 * TASK STATES
 * 
 * Each task can be in one of these states:
 * - READY: Task is ready to run
 * - RUNNING: Task is currently executing
 * - BLOCKED: Task is waiting for an event
 * - SUSPENDED: Task is suspended by user
 * - TERMINATED: Task has finished execution
 */
typedef enum {
    TASK_READY = 0,
    TASK_RUNNING,
    TASK_BLOCKED,
    TASK_SUSPENDED,
    TASK_TERMINATED
} task_state_t;

/*
 * TASK CONTROL BLOCK (TCB)
 * 
 * Contains all information needed to manage a task.
 * This is the core data structure of the RTOS.
 */
typedef struct task_control_block {
    // Task identification
    uint32_t task_id;                    // Unique task identifier
    char name[16];                       // Task name for debugging
    
    // CPU context (registers)
    uint32_t* stack_pointer;             // Current stack pointer
    uint32_t registers[16];              // Saved CPU registers (R0-R15)
    
    // Task properties
    uint8_t priority;                    // Task priority (0 = highest)
    task_state_t state;                  // Current task state
    uint32_t time_slice_remaining;       // Remaining time slice
    
    // Stack management
    uint8_t* stack_base;                 // Base of task stack
    uint32_t stack_size;                 // Size of task stack
    
    // Timing and statistics
    uint32_t total_runtime;              // Total CPU time used
    uint32_t last_run_time;              // When task last ran
    uint32_t wake_time;                  // When to wake up (if sleeping)
    
    // Linked list pointers
    struct task_control_block* next;     // Next task in list
    struct task_control_block* prev;     // Previous task in list
    
    // Task function
    void (*task_function)(void* param);  // Task entry point
    void* task_parameter;                // Parameter for task function
} tcb_t;

/*
 * SCHEDULER STATISTICS
 */
typedef struct {
    uint32_t total_context_switches;     // Total context switches
    uint32_t total_ticks;                // Total system ticks
    uint32_t idle_time;                  // Time spent in idle task
    uint32_t tasks_created;              // Number of tasks created
    uint32_t tasks_deleted;              // Number of tasks deleted
} scheduler_stats_t;

/*
 * RTOS KERNEL FUNCTIONS
 */

/**
 * Initialize the RTOS kernel
 * @return: 0 on success, -1 on failure
 */
int rtos_init(void);

/**
 * Start the RTOS scheduler
 * This function never returns
 */
void rtos_start(void);

/**
 * Create a new task
 * @param name: Task name (for debugging)
 * @param task_func: Task function pointer
 * @param param: Parameter to pass to task
 * @param priority: Task priority (0 = highest)
 * @param stack_size: Stack size in bytes
 * @return: Task ID on success, 0 on failure
 */
uint32_t task_create(const char* name, 
                     void (*task_func)(void* param),
                     void* param,
                     uint8_t priority,
                     uint32_t stack_size);

/**
 * Delete a task
 * @param task_id: Task ID to delete
 * @return: 0 on success, -1 on failure
 */
int task_delete(uint32_t task_id);

/**
 * Suspend a task
 * @param task_id: Task ID to suspend
 * @return: 0 on success, -1 on failure
 */
int task_suspend(uint32_t task_id);

/**
 * Resume a suspended task
 * @param task_id: Task ID to resume
 * @return: 0 on success, -1 on failure
 */
int task_resume(uint32_t task_id);

/**
 * Change task priority
 * @param task_id: Task ID
 * @param new_priority: New priority level
 * @return: 0 on success, -1 on failure
 */
int task_set_priority(uint32_t task_id, uint8_t new_priority);

/*
 * TASK CONTROL FUNCTIONS
 */

/**
 * Yield CPU to other tasks (voluntary context switch)
 */
void task_yield(void);

/**
 * Put current task to sleep for specified milliseconds
 * @param ms: Sleep duration in milliseconds
 */
void task_sleep(uint32_t ms);

/**
 * Get current task ID
 * @return: Current task ID
 */
uint32_t task_get_current_id(void);

/**
 * Get task information
 * @param task_id: Task ID
 * @return: Pointer to TCB, NULL if not found
 */
tcb_t* task_get_info(uint32_t task_id);

/*
 * SCHEDULER FUNCTIONS
 */

/**
 * Force a context switch (called by timer interrupt)
 */
void scheduler_tick(void);

/**
 * Get next task to run based on scheduling algorithm
 * @return: Pointer to next task's TCB
 */
tcb_t* scheduler_get_next_task(void);

/**
 * Perform context switch between tasks
 * @param current_task: Currently running task
 * @param next_task: Task to switch to
 */
void context_switch(tcb_t* current_task, tcb_t* next_task);

/*
 * SYNCHRONIZATION PRIMITIVES
 */

/**
 * Mutex structure
 */
typedef struct {
    bool locked;                         // Mutex state
    uint32_t owner_task_id;             // Task that owns the mutex
    tcb_t* waiting_tasks;               // Queue of waiting tasks
} mutex_t;

/**
 * Semaphore structure
 */
typedef struct {
    int32_t count;                      // Semaphore count
    tcb_t* waiting_tasks;               // Queue of waiting tasks
} semaphore_t;

/**
 * Initialize a mutex
 * @param mutex: Mutex to initialize
 */
void mutex_init(mutex_t* mutex);

/**
 * Lock a mutex
 * @param mutex: Mutex to lock
 * @return: 0 on success, -1 on failure
 */
int mutex_lock(mutex_t* mutex);

/**
 * Unlock a mutex
 * @param mutex: Mutex to unlock
 * @return: 0 on success, -1 on failure
 */
int mutex_unlock(mutex_t* mutex);

/**
 * Initialize a semaphore
 * @param sem: Semaphore to initialize
 * @param initial_count: Initial count value
 */
void semaphore_init(semaphore_t* sem, int32_t initial_count);

/**
 * Wait on a semaphore (P operation)
 * @param sem: Semaphore to wait on
 * @return: 0 on success, -1 on failure
 */
int semaphore_wait(semaphore_t* sem);

/**
 * Signal a semaphore (V operation)
 * @param sem: Semaphore to signal
 * @return: 0 on success, -1 on failure
 */
int semaphore_signal(semaphore_t* sem);

/*
 * UTILITY AND DEBUG FUNCTIONS
 */

/**
 * Get scheduler statistics
 * @return: Pointer to scheduler statistics
 */
scheduler_stats_t* get_scheduler_stats(void);

/**
 * Print all task information (for debugging)
 */
void print_task_list(void);

/**
 * Print scheduler statistics
 */
void print_scheduler_stats(void);

/**
 * Get system uptime in milliseconds
 * @return: System uptime
 */
uint32_t get_system_uptime(void);

/**
 * Get CPU utilization percentage
 * @return: CPU utilization (0-100%)
 */
uint8_t get_cpu_utilization(void);

/*
 * MEMORY MANAGEMENT
 */

/**
 * Allocate memory from RTOS heap
 * @param size: Size in bytes
 * @return: Pointer to allocated memory, NULL on failure
 */
void* rtos_malloc(size_t size);

/**
 * Free memory allocated by rtos_malloc
 * @param ptr: Pointer to memory to free
 */
void rtos_free(void* ptr);

/**
 * Get free heap size
 * @return: Free heap size in bytes
 */
size_t get_free_heap_size(void);

#endif // RTOS_H