#include <stdio.h>
#include <pthread.h>

#define ITERATIONS 10000

int counter = 0;
pthread_mutex_t lock;

// 1. UNSAFE FUNCTION (Race Condition)
void* unsafe_increment(void* arg) {
    for (int i = 0; i < ITERATIONS; i++) {
        // CRITICAL SECTION (Unprotected)
        // CPU performs 3 assembly steps: Read -> Modify -> Write
        counter++; 
    }
    return NULL;
}

// 2. SAFE FUNCTION (Synchronized with Mutex)
/*void* safe_increment(void* arg) {
    for (int i = 0; i < ITERATIONS; i++) {
        pthread_mutex_lock(&lock);   // Acquire Lock
        
        // CRITICAL SECTION (Protected)
        counter++;                   
        
        pthread_mutex_unlock(&lock); // Release Lock
    }
    return NULL;
}*/

int main() {
    pthread_t thread1, thread2;

    // --- DEMO 1: RACE CONDITION ---
    printf("--- Running Unsynchronized (Race Condition) ---\n");
    counter = 0;
    pthread_create(&thread1, NULL, unsafe_increment, NULL);
    pthread_create(&thread2, NULL, unsafe_increment, NULL);
    
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    printf("Expected Counter: %d\n", ITERATIONS * 2);
    printf("Actual Counter:   %d (Lost updates due to race condition!)\n\n", counter);

    // --- DEMO 2: SYNCHRONIZATION ---
/*    printf("--- Running Synchronized (Mutex Lock) ---\n");
    counter = 0;
    pthread_mutex_init(&lock, NULL); // Initialize mutex

    pthread_create(&thread1, NULL, safe_increment, NULL);
    pthread_create(&thread2, NULL, safe_increment, NULL);
    
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    pthread_mutex_destroy(&lock); // Destroy mutex

    printf("Expected Counter: %d\n", ITERATIONS * 2);
    printf("Actual Counter:   %d (Correct output!)\n", counter);
*/
    return 0;
}

/* OUTPUT
~$ cd -- "$HOME" && gcc 'process_synchronization.c' -o ./'process_synchronization' && ./'process_synchronization'
--- Running Unsynchronized (Race Condition) ---
Expected Counter: 20
Actual Counter:   20 (Lost updates due to race condition!)

~$ cd -- "$HOME" && gcc 'process_synchronization.c' -o ./'process_synchronization' && ./'process_synchronization'
--- Running Unsynchronized (Race Condition) ---
Expected Counter: 200
Actual Counter:   200 (Lost updates due to race condition!)

~$ cd -- "$HOME" && gcc 'process_synchronization.c' -o ./'process_synchronization' && ./'process_synchronization'
--- Running Unsynchronized (Race Condition) ---
Expected Counter: 200
Actual Counter:   200 (Lost updates due to race condition!)

~$ cd -- "$HOME" && gcc 'process_synchronization.c' -o ./'process_synchronization' && ./'process_synchronization'
--- Running Unsynchronized (Race Condition) ---
Expected Counter: 2000
Actual Counter:   2000 (Lost updates due to race condition!)

~$ cd -- "$HOME" && gcc 'process_synchronization.c' -o ./'process_synchronization' && ./'process_synchronization'
--- Running Unsynchronized (Race Condition) ---
Expected Counter: 20000
Actual Counter:   15377 (Lost updates due to race condition!)
*/
