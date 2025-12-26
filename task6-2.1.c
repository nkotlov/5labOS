#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <errno.h>

#define MAX_CMD 1024
#define MAX_ARGS 64

void handle_sigchld(int sig) {
    int status;
    pid_t pid;
    
    while ((pid = waitpid(-1, &status, WNOHANG)) > 0) {
        if (WIFEXITED(status)) {
            printf("[parent] child=%d exited, code=%d\n", pid, WEXITSTATUS(status));
            printf("shell>>> ");
            fflush(stdout);
        }
    }
}

int main() {
    char cmd[MAX_CMD];
    char cmd_copy[MAX_CMD];
    char *args[MAX_ARGS];
    pid_t pid;
    
    struct sigaction sa;
    sa.sa_handler = handle_sigchld;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART | SA_NOCLDSTOP;
    
    if (sigaction(SIGCHLD, &sa, NULL) == -1) {
        perror("sigaction failed");
        exit(1);
    }
    
    printf("shell>>> ");
    fflush(stdout);
    
    while (1) {
        if (fgets(cmd, MAX_CMD, stdin) == NULL) {
            printf("\n");
            break;
        }
        
        cmd[strcspn(cmd, "\n")] = '\0';
        
        if (strlen(cmd) == 0) {
            printf("shell>>> ");
            fflush(stdout);
            continue;
        }
        
        if (strcmp(cmd, "exit") == 0) {
            break;
        }
        
        strcpy(cmd_copy, cmd);
        
        int i = 0;
        args[i] = strtok(cmd_copy, " ");
        while (args[i] != NULL && i < MAX_ARGS - 1) {
            i++;
            args[i] = strtok(NULL, " ");
        }
        args[i] = NULL;
        
        if (args[0] == NULL) {
            printf("shell>>> ");
            fflush(stdout);
            continue;
        }
        
        pid = fork();
        
        if (pid == -1) {
            perror("fork failed");
            printf("shell>>> ");
            fflush(stdout);
            continue;
        }
        
        if (pid == 0) {
            printf("[child] pid=%d  ppid=%d  cmd=%s\n", getpid(), getppid(), args[0]);
            fflush(stdout);
            
            execvp(args[0], args);
            fprintf(stderr, "Ошибка выполнения '%s': ", args[0]);
            perror("");
            exit(EXIT_FAILURE);
        } else {
            printf("[parent] pid=%d created child=%d\n", getpid(), pid);
            fflush(stdout);
            printf("shell>>> ");
            fflush(stdout);
            usleep(50000);
        }
    }
    
    while (wait(NULL) > 0);
    
    return 0;
}