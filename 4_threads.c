// Copied from a bot
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

// This is the function that the new thread will execute
void* myThreadFunction(void* arg) {
    char* message = (char*)arg;
    
    for (int i = 0; i < 3; i++) {
        printf("Thread says: %s (Loop %d)\n", message, i + 1);
        sleep(1); // Suspend execution for 1 second
    }
    
    return NULL;
}

int main() {
    pthread_t thread_id; // Variable to hold the thread identifier
    char* msg = "Hello from the custom thread!";
    
    printf("Main program: Creating the new thread...\n");
    
    // 1. Create the thread
    if (pthread_create(&thread_id, NULL, myThreadFunction, (void*)msg) != 0) {
        perror("Failed to create thread");
        return 1;
    }
    
    // 2. The main thread continues executing its own code simultaneously
    for (int i = 0; i < 2; i++) {
        printf("Main program: Doing other work (Loop %d)\n", i + 1);
        sleep(1);
    }
    
    printf("Main program: Waiting for the custom thread to finish...\n");
    
    // 3. Wait for the thread to complete before exiting
    pthread_join(thread_id, NULL);
    
    printf("Main program: Thread finished. Exiting process.\n");
    return 0;
}

/* OUTPUT
Main program: Creating the new thread...
Main program: Doing other work (Loop 1)
Thread says: Hello from the custom thread! (Loop 1)
Main program: Doing other work (Loop 2)
Thread says: Hello from the custom thread! (Loop 2)
Main program: Waiting for the custom thread to finish...
Thread says: Hello from the custom thread! (Loop 3)
Main program: Thread finished. Exiting process.
*/
