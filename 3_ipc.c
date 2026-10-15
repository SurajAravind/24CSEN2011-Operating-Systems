#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

int main() {
    // 1. Interprocess Communication (IPC) via Unnamed Pipe
    // fd[0] = read end, fd[1] = write end
    int fd[2];
    pid_t pid;
    char write_msg[] = "Hello from Child Process via Pipe!";
    char read_msg[100];

    // Create the pipe
    if (pipe(fd) == -1) {
        perror("Pipe creation failed");
        exit(1);
    }

    // 2. Process Creation (Process Management using fork)
    pid = fork();

    if (pid < 0) {
        // Fork failed
        perror("Fork failed");
        exit(1);
    } 
    else if (pid == 0) {
        // ==================== CHILD PROCESS ====================
        printf("[Child] Child process created. PID: %d, Parent PID: %d\n", getpid(), getppid());
        
        // Close the unused read end of the pipe in child
        close(fd[0]);

        // Write a message to the pipe
        write(fd[1], write_msg, strlen(write_msg) + 1);
        printf("[Child] Sent message to parent: \"%s\"\n", write_msg);

        // Close the write end
        close(fd[1]);
        exit(0); // Terminate child process
    } 
    else {
        // ==================== PARENT PROCESS ====================
        // Parent waits for the child process to complete (Scheduling / Synchronization via wait)
        int status;
        wait(&status);
        printf("[Parent] Child process (PID: %d) has finished execution.\n", pid);

        // Close the unused write end of the pipe in parent
        close(fd[1]);

        // Read the message from the child via the pipe
        read(fd[0], read_msg, sizeof(read_msg));
        printf("[Parent] Received message from child: \"%s\"\n", read_msg);

        // Close the read end
        close(fd[0]);
    }

    return 0;
}

//OUTPUT
//$ cd -- "$HOME" && gcc 'process_management.c' -o ./'process_management' && ./'process_management'
//[Child] Child process created. PID: 100, Parent PID: 99
//[Child] Sent message to parent: "Hello from Child Process via Pipe!"
//[Parent] Child process (PID: 100) has finished execution.
//[Parent] Received message from child: "Hello from Child Process via Pipe!"
