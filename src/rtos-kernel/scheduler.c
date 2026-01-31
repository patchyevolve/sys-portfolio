/*
 * RTOS Scheduler Implementation
 * 
 * This file implements the core scheduling algorithm for our RTOS.
 * We use a priority-based round-robin scheduler with preemption.
 * 
 * SCHEDULING ALGORITHM:
 * 1. Tasks are organized by priority levels (0 = highest priority)
 * 2. Within each priority level, tasks are scheduled round-robin
 * 3. Higher priority tasks preempt lower priority tasks
 * 4. Time slicing prevents task starvation within same priority
 */

#include "rtos.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * GLOBAL SCHEDULER STATE
 */
static tcb_t* ready_queues[PRIORITY_LEVELS];  // Ready queues per priority
static tcb_t* current_task = NULL;            // Currently running task
static tcb_t* idle_task = NULL;               // Idle task (always ready)
static uint32_t next_task_id = 1;             // Next available task ID
static bool scheduler_running = false;         // Scheduler state
static uint32_t system_tick_count = 0;        // System tick counter
static scheduler_stats_t stats = {0};         // Scheduler statistics

/*
 * TASK STORAGE
 * In a real RTOS, this would be dynamically allocated
 */
static tcb_t task_pool[MAX_TASKS];            // Pre-allocated task pool
static uint8_t task_stacks[MAX_TASKS][STACK_SIZE]; // Pre-allocated stacks
static bool task_pool_used[MAX_TASKS] = {false}; // Track used slots

/*
 * IDLE TASK
 * 
 * The idle task runs when no other tasks are ready.
 * It's the lowest priority task and helps measure CPU utilization.
 */
static void idle_task_function(void* param) {
    (void)param; // Unused parameter
    
    while (1) {
        // In a real system, this might:
        // - Put CPU in low-power mode
        // - Perform background garbage collection
        // - Update system statistics
        
        stats.idle_time++;
        
        // Simulate some idle work
        for (volatile int i = 0; i < 1000; i++) {
            // Busy wait to simulate idle processing
        }
        
        // Yield to allow other tasks to run
        task_yield();
    }
}

/*
 * SCHEDULER INITIALIZATION
 */
int rtos_init(void) {
    printf("🚀 Initializing RTOS kernel\n");
    
    // Initialize ready queues
    for (int i = 0; i < PRIORITY_LEVELS; i++) {
        ready_queues[i] = NULL;
    }
    
    // Initialize task pool
    memset(task_pool, 0, sizeof(task_pool));
    memset(task_pool_used, false, sizeof(task_pool_used));
    
    // Initialize statistics
    memset(&stats, 0, sizeof(stats));
    
    // Create idle task (lowest priority)
    uint32_t idle_id = task_create("IDLE", idle_task_function, NULL, 
                                   PRIORITY_LEVELS - 1, STACK_SIZE);
    if (idle_id == 0) {
        printf("❌ Failed to create idle task\n");
        return -1;
    }
    
    idle_task = task_get_info(idle_id);
    
    printf("✅ RTOS kernel initialized\n");
    printf("   Max tasks: %d\n", MAX_TASKS);
    printf("   Priority levels: %d\n", PRIORITY_LEVELS);
    printf("   Time slice: %d ms\n", TIME_SLICE_MS);
    
    return 0;
}

/*
 * TASK CREATION
 * 
 * Creates a new task and adds it to the appropriate ready queue.
 */
uint32_t task_create(const char* name, 
                     void (*task_func)(void* param),
                     void* param,
                     uint8_t priority,
                     uint32_t stack_size) {
    
    printf("📋 Creating task '%s' (priority %d)\n", name, priority);
    
    // Validate parameters
    if (priority >= PRIORITY_LEVELS) {
        printf("❌ Invalid priority: %d (max: %d)\n", priority, PRIORITY_LEVELS - 1);
        return 0;
    }
    
    if (stack_size > STACK_SIZE) {
        printf("❌ Stack size too large: %u (max: %d)\n", stack_size, STACK_SIZE);
        return 0;
    }
    
    // Find free slot in task pool
    int slot = -1;
    for (int i = 0; i < MAX_TASKS; i++) {
        if (!task_pool_used[i]) {
            slot = i;
            break;
        }
    }
    
    if (slot == -1) {
        printf("❌ No free task slots available\n");
        return 0;
    }
    
    // Initialize TCB
    tcb_t* tcb = &task_pool[slot];
    task_pool_used[slot] = true;
    
    tcb->task_id = next_task_id++;
    strncpy(tcb->name, name, sizeof(tcb->name) - 1);
    tcb->name[sizeof(tcb->name) - 1] = '\0';
    
    tcb->priority = priority;
    tcb->state = TASK_READY;
    tcb->time_slice_remaining = TIME_SLICE_MS;
    
    // Initialize stack
    tcb->stack_base = task_stacks[slot];
    tcb->stack_size = stack_size;
    tcb->stack_pointer = (uint32_t*)(tcb->stack_base + stack_size - 4);
    
    // Initialize registers (simulated ARM Cortex-M context)
    memset(tcb->registers, 0, sizeof(tcb->registers));
    tcb->registers[15] = (uint32_t)task_func;  // PC (Program Counter)
    tcb->registers[14] = 0xFFFFFFFD;           // LR (Link Register - return to thread mode)
    tcb->registers[13] = (uint32_t)tcb->stack_pointer; // SP (Stack Pointer)
    tcb->registers[0] = (uint32_t)param;       // R0 (first parameter)
    
    // Initialize timing
    tcb->total_runtime = 0;
    tcb->last_run_time = system_tick_count;
    tcb->wake_time = 0;
    
    // Initialize linked list pointers
    tcb->next = NULL;
    tcb->prev = NULL;
    
    // Store task function and parameter
    tcb->task_function = task_func;
    tcb->task_parameter = param;
    
    // Add to ready queue
    add_task_to_ready_queue(tcb);
    
    stats.tasks_created++;
    
    printf("✅ Task '%s' created (ID: %u, slot: %d)\n", name, tcb->task_id, slot);
    
    return tcb->task_id;
}

/*
 * ADD TASK TO READY QUEUE
 * 
 * Adds a task to the appropriate priority ready queue.
 */
void add_task_to_ready_queue(tcb_t* task) {
    if (task->state != TASK_READY) {
        task->state = TASK_READY;
    }
    
    uint8_t priority = task->priority;
    
    // Add to end of priority queue (round-robin within priority)
    if (ready_queues[priority] == NULL) {
        // First task in this priority queue
        ready_queues[priority] = task;
        task->next = task;
        task->prev = task;
    } else {
        // Insert at end of circular list
        tcb_t* last = ready_queues[priority]->prev;
        
        task->next = ready_queues[priority];
        task->prev = last;
        last->next = task;
        ready_queues[priority]->prev = task;
    }
}

/*
 * REMOVE TASK FROM READY QUEUE
 */
void remove_task_from_ready_queue(tcb_t* task) {
    uint8_t priority = task->priority;
    
    if (ready_queues[priority] == NULL) {
        return; // Task not in queue
    }
    
    if (task->next == task) {
        // Only task in queue
        ready_queues[priority] = NULL;
    } else {
        // Remove from circular list
        task->prev->next = task->next;
        task->next->prev = task->prev;
        
        // Update queue head if necessary
        if (ready_queues[priority] == task) {
            ready_queues[priority] = task->next;
        }
    }
    
    task->next = NULL;
    task->prev = NULL;
}

/*
 * SCHEDULER - GET NEXT TASK
 * 
 * Implements priority-based scheduling with round-robin within priorities.
 */
tcb_t* scheduler_get_next_task(void) {
    // Find highest priority queue with ready tasks
    for (int priority = 0; priority < PRIORITY_LEVELS; priority++) {
        if (ready_queues[priority] != NULL) {
            tcb_t* next_task = ready_queues[priority];
            
            // Move to next task in round-robin fashion
            ready_queues[priority] = next_task->next;
            
            return next_task;
        }
    }
    
    // Should never happen if idle task exists
    return idle_task;
}

/*
 * CONTEXT SWITCH
 * 
 * Switches from current task to next task.
 * In a real system, this would save/restore CPU registers.
 */
void context_switch(tcb_t* current, tcb_t* next) {
    if (current == next) {
        return; // No switch needed
    }
    
    printf("🔄 Context switch: %s -> %s\n", 
           current ? current->name : "NULL", 
           next ? next->name : "NULL");
    
    // Update statistics
    stats.total_context_switches++;
    
    // Save current task context (simulated)
    if (current != NULL) {
        // In real ARM Cortex-M, this would be done by hardware/PendSV
        // We simulate by storing the "current" register state
        current->last_run_time = system_tick_count;
        
        // Update runtime statistics
        if (current->state == TASK_RUNNING) {
            current->total_runtime++;
        }
        
        // If task is still ready, put it back in queue
        if (current->state == TASK_RUNNING) {
            current->state = TASK_READY;
            add_task_to_ready_queue(current);
        }
    }
    
    // Load next task context
    if (next != NULL) {
        current_task = next;
        next->state = TASK_RUNNING;
        next->time_slice_remaining = TIME_SLICE_MS;
        
        // Remove from ready queue
        remove_task_from_ready_queue(next);
        
        // In real system, restore CPU registers from next->registers[]
        // and set stack pointer to next->stack_pointer
    }
}

/*
 * SCHEDULER TICK
 * 
 * Called by system timer interrupt (simulated).
 * Handles time slicing and task switching.
 */
void scheduler_tick(void) {
    system_tick_count++;
    stats.total_ticks++;
    
    if (!scheduler_running || current_task == NULL) {
        return;
    }
    
    // Update current task's time slice
    if (current_task->time_slice_remaining > 0) {
        current_task->time_slice_remaining--;
    }
    
    // Check for sleeping tasks to wake up
    wake_sleeping_tasks();
    
    // Check if current task's time slice expired or higher priority task ready
    bool need_reschedule = false;
    
    // Time slice expiration
    if (current_task->time_slice_remaining == 0) {
        printf("⏰ Time slice expired for task %s\n", current_task->name);
        need_reschedule = true;
    }
    
    // Higher priority task became ready
    tcb_t* highest_ready = get_highest_priority_ready_task();
    if (highest_ready && highest_ready->priority < current_task->priority) {
        printf("⚡ Higher priority task %s preempting %s\n", 
               highest_ready->name, current_task->name);
        need_reschedule = true;
    }
    
    // Perform context switch if needed
    if (need_reschedule) {
        tcb_t* next_task = scheduler_get_next_task();
        context_switch(current_task, next_task);
    }
}

/*
 * GET HIGHEST PRIORITY READY TASK
 */
tcb_t* get_highest_priority_ready_task(void) {
    for (int priority = 0; priority < PRIORITY_LEVELS; priority++) {
        if (ready_queues[priority] != NULL) {
            return ready_queues[priority];
        }
    }
    return NULL;
}

/*
 * WAKE SLEEPING TASKS
 * 
 * Checks for tasks that should wake up from sleep.
 */
void wake_sleeping_tasks(void) {
    for (int i = 0; i < MAX_TASKS; i++) {
        if (task_pool_used[i]) {
            tcb_t* task = &task_pool[i];
            
            if (task->state == TASK_BLOCKED && 
                task->wake_time > 0 && 
                system_tick_count >= task->wake_time) {
                
                printf("😴 Waking up task %s\n", task->name);
                task->wake_time = 0;
                task->state = TASK_READY;
                add_task_to_ready_queue(task);
            }
        }
    }
}

/*
 * START SCHEDULER
 * 
 * Begins task execution. This function never returns.
 */
void rtos_start(void) {
    printf("🎯 Starting RTOS scheduler\n");
    
    scheduler_running = true;
    
    // Get first task to run
    tcb_t* first_task = scheduler_get_next_task();
    if (first_task == NULL) {
        printf("❌ No tasks to run!\n");
        return;
    }
    
    printf("🏃 Starting with task: %s\n", first_task->name);
    
    // Start first task
    context_switch(NULL, first_task);
    
    // Simulate scheduler main loop
    // In a real system, this would be handled by timer interrupts
    while (scheduler_running) {
        // Simulate timer tick every 1ms
        scheduler_tick();
        
        // Simulate task execution
        if (current_task && current_task->task_function) {
            // In real system, task would run until preempted
            // Here we simulate by calling task function briefly
            printf("🔄 Running task %s\n", current_task->name);
        }
        
        // Small delay to make output readable
        usleep(1000); // 1ms delay
    }
}

/*
 * TASK CONTROL FUNCTIONS
 */

void task_yield(void) {
    if (!scheduler_running || current_task == NULL) {
        return;
    }
    
    printf("🤝 Task %s yielding CPU\n", current_task->name);
    
    // Force time slice to expire
    current_task->time_slice_remaining = 0;
    
    // Trigger reschedule
    scheduler_tick();
}

void task_sleep(uint32_t ms) {
    if (!scheduler_running || current_task == NULL) {
        return;
    }
    
    printf("😴 Task %s sleeping for %u ms\n", current_task->name, ms);
    
    // Set wake time
    current_task->wake_time = system_tick_count + ms;
    current_task->state = TASK_BLOCKED;
    
    // Remove from ready queue and switch to next task
    tcb_t* next_task = scheduler_get_next_task();
    context_switch(current_task, next_task);
}

uint32_t task_get_current_id(void) {
    return current_task ? current_task->task_id : 0;
}

tcb_t* task_get_info(uint32_t task_id) {
    for (int i = 0; i < MAX_TASKS; i++) {
        if (task_pool_used[i] && task_pool[i].task_id == task_id) {
            return &task_pool[i];
        }
    }
    return NULL;
}

/*
 * UTILITY FUNCTIONS
 */

scheduler_stats_t* get_scheduler_stats(void) {
    return &stats;
}

void print_task_list(void) {
    printf("\n📋 TASK LIST\n");
    printf("============\n");
    printf("ID   Name         State      Priority  Runtime  Stack\n");
    printf("---  -----------  ---------  --------  -------  -----\n");
    
    for (int i = 0; i < MAX_TASKS; i++) {
        if (task_pool_used[i]) {
            tcb_t* task = &task_pool[i];
            const char* state_str;
            
            switch (task->state) {
                case TASK_READY: state_str = "READY"; break;
                case TASK_RUNNING: state_str = "RUNNING"; break;
                case TASK_BLOCKED: state_str = "BLOCKED"; break;
                case TASK_SUSPENDED: state_str = "SUSPENDED"; break;
                case TASK_TERMINATED: state_str = "TERMINATED"; break;
                default: state_str = "UNKNOWN"; break;
            }
            
            printf("%3u  %-11s  %-9s  %8u  %7u  %5u\n",
                   task->task_id, task->name, state_str,
                   task->priority, task->total_runtime, task->stack_size);
        }
    }
}

void print_scheduler_stats(void) {
    printf("\n📊 SCHEDULER STATISTICS\n");
    printf("=======================\n");
    printf("System uptime:      %u ticks\n", system_tick_count);
    printf("Context switches:   %u\n", stats.total_context_switches);
    printf("Tasks created:      %u\n", stats.tasks_created);
    printf("Tasks deleted:      %u\n", stats.tasks_deleted);
    printf("Idle time:          %u ticks\n", stats.idle_time);
    printf("CPU utilization:    %u%%\n", get_cpu_utilization());
}

uint32_t get_system_uptime(void) {
    return system_tick_count;
}

uint8_t get_cpu_utilization(void) {
    if (stats.total_ticks == 0) {
        return 0;
    }
    
    uint32_t busy_time = stats.total_ticks - stats.idle_time;
    return (uint8_t)((busy_time * 100) / stats.total_ticks);
}