#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

int main() {
    pid_t pid = fork();  //fork() is a system call

    if (pid < 0) {
        perror("Fork failed");
        exit(EXIT_FAILURE);
    } 
    else if (pid == 0) {
        // --- CHILD PROCESS ---
        char message[] = "Hello from Child Process using write system call!\n";
        
        write(1, message, sizeof(message) - 1);  // write is a system call
        exit(EXIT_SUCCESS); 
    } 
    else {
        // --- PARENT PROCESS ---
        wait(NULL); // wait is s system call
        
        char buffer[100];
        int fd = open("example.txt", O_RDONLY);  // open is a system call
        if (fd < 0) {
           char err[] = "Parent: Could not open example.txt (file missing?)\n";
            write(2, err, sizeof(err) - 1); // write to stderr (fd 2)
            exit(EXIT_FAILURE);
        }

        ssize_t bytesRead = read(fd, buffer, sizeof(buffer) - 1);  //read is a system call
        if (bytesRead >= 0) {
            buffer[bytesRead] = '\0'; // Null-terminate string
            printf("\nParent read from file: %s", buffer);
        }

        close(fd);  // close is a system call
    }

    return 0;
}


// OUTPUT
//~$ cd -- "$HOME" && gcc 'sample.c' -o ./'sample' && ./'sample'
//Hello from Child Process using write system call!

//Parent read from file: Hello.  This file is created for demonstration purpose.
