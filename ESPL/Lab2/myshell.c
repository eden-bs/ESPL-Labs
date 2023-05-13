#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/wait.h>
#include "LineParser.h"




void execute(cmdLine *pCmdLine) {
    if (strcmp(pCmdLine->arguments[0], "cd") == 0){
        fprintf(stderr, "Current process with PID = %d\n", getpid());
        fprintf(stderr, "Executing command: %s\n", pCmdLine->arguments[0]);
        if (chdir(pCmdLine->arguments[1]) != 0) {
            perror("chdir");
        }
    }
    else if(strcmp(pCmdLine->arguments[0], "suspend") == 0){
        if ((kill(atoi(pCmdLine->arguments[1]), SIGTSTP)) == 0) {
            printf("Process %s has been suspended.\n", pCmdLine->arguments[1]);
        } else {
            printf("Failed to suspend process %s.\n", pCmdLine->arguments[1]);
        }
    }
    else if(strcmp(pCmdLine->arguments[0], "wake") == 0){
        if ((kill(atoi(pCmdLine->arguments[1]), SIGCONT)) == 0) {
            printf("Process  %s has been woken up.\n", pCmdLine->arguments[1]);
        } else {
            printf("Failed to wake up process  %s.\n", pCmdLine->arguments[1]);
        }
    }
    else if(strcmp(pCmdLine->arguments[0], "kill") == 0){
        if ((kill(atoi(pCmdLine->arguments[1]), SIGTERM)) == 0) {
            printf("Process  %s has been terminated.\n", pCmdLine->arguments[1]);
        } else {
            printf("Failed to terminate process  %s.\n", pCmdLine->arguments[1]);
        }
    }
    else{
        pid_t pid = fork();
        if (pid == -1) {
            perror("fork");
            exit(EXIT_FAILURE);
        } else if (pid == 0) { 
            fprintf(stderr, "Child process with PID = %d\n", getpid());
            fprintf(stderr, "Executing command: %s\n", pCmdLine->arguments[0]);
            
            if (pCmdLine->inputRedirect != NULL) {
                int fd = open(pCmdLine->inputRedirect, O_RDONLY);
                if (fd == -1) {
                    fprintf(stderr, "Failed to open input file '%s'\n", pCmdLine->inputRedirect);
                    _exit(1);
                }
                dup2(fd, STDIN_FILENO);
                close(fd);
            }
            
            if (pCmdLine->outputRedirect != NULL) {
                int fd = open(pCmdLine->outputRedirect, O_WRONLY | O_CREAT );
                if (fd == -1) {
                    fprintf(stderr, "Failed to open output file '%s'\n", pCmdLine->outputRedirect);
                    _exit(1);
                }
                dup2(fd, STDOUT_FILENO);
                close(fd);
            }
            if (execvp(pCmdLine->arguments[0], pCmdLine->arguments) == -1) {
                perror("execvp");
                _exit(EXIT_FAILURE);
            }
        } else { 
            int status;
            if (pCmdLine->blocking)
                waitpid(pid, &status, 0);   
        }
    }
}
int main() {
        char cwd[2048]; // buffer to store the current working directory
        while(1){
        if (getcwd(cwd, sizeof(cwd)) == NULL){
            perror("getcwd illegal");
            exit(0);
        }
        printf("%s\n",cwd);
        char input[2048]; // buffer to store user input
        fgets(input, sizeof(input), stdin);
        if(input == NULL){
            perror("illegal command");
            exit(0);
        }
        cmdLine *pCmdLine = parseCmdLines(input);
        execute(pCmdLine);
        freeCmdLines(pCmdLine);
    }
    return 0;
}


