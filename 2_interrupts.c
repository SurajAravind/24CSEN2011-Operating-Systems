#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

// This is our Signal Handler (the user-space equivalent of an ISR)
void handle_sigint(int sig) {
    // Note: It is generally unsafe to use printf inside real handlers; 
    // write() is used because it is an asynchronous-safe system call.
    char msg[] = "\n[Interrupt Caught!] Safely intercepting Ctrl+C (SIGINT).\n";
    write(1, msg, sizeof(msg) - 1);
    
    char exit_msg[] = "Exiting program cleanly.\n";
    write(1, exit_msg, sizeof(exit_msg) - 1);
    
    exit(0); 
}

int main() {
    // Register our custom handler for the SIGINT signal
    // Under the hood, this uses the signal/sigaction system call to notify the kernel
    if (signal(SIGINT, handle_sigint) == SIG_ERR) {
        perror("Failed to register signal handler");
        return 1;
    }

    printf("Program running. Try pressing Ctrl+C to trigger a hardware/software interrupt...\n");

    // Infinite loop representing a continuous low-priority background task
    while (1) {
        printf("Processing background tasks... (Looping)\n");
        sleep(2); // Sleeps to prevent CPU maxing out
    }

    return 0;
}

//OUTPUT

//~$ cd -- "$HOME" && gcc 'interrupts_prog.c' -o ./'interrupts_prog' && ./'interrupts_prog'
//Program running. Try pressing Ctrl+C to trigger a hardware/software interrupt...
//Processing background tasks... (Looping)
//Processing background tasks... (Looping)
//Processing background tasks... (Looping)
//Processing background tasks... (Looping)
//^C
//[Interrupt Caught!] Safely intercepting Ctrl+C (SIGINT).
Exiting program cleanly.
