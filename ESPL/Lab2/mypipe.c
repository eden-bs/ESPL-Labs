#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>



int main() {
    int pipefd[2];
    pid_t pid;
    char buffer[1024];
    
    if (pipe(pipefd) == -1) {
        perror("pipe");
        exit(1);
    }

    // Fork a child process
    pid = fork();

    if (pid == -1) {
        perror("fork");
        exit(0);
    } else if (pid == 0) {
        // Child process
        close(pipefd[0]);
        write(pipefd[1], "hello", 5);
        close(pipefd[1]);
        exit(1);
    } else {
        // Parent process
        close(pipefd[1]);
        
        ssize_t num_bytes = read(pipefd[0], buffer, 1024);

        if (num_bytes > 0) {
            printf("Incoming message: %.*s\n", (int)num_bytes, buffer);
        } else if (num_bytes == 0) {
            printf("No data received.\n");
        } else {
            perror("read");
            exit(1);
        }

        // Close the read end of the pipe
        close(pipefd[0]);
        
        exit(0);
    }
}