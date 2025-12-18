#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_CMD 1024
#define MAX_ARGS 64

int main() {
    char cmd[MAX_CMD];
    char *args[MAX_ARGS];
    
    while (1) {
        printf("> ");
        fflush(stdout);
        
        if (fgets(cmd, MAX_CMD, stdin) == NULL) {
            break;
        }
        
        cmd[strcspn(cmd, "\n")] = '\0';
        
        if (strlen(cmd) == 0) {
            continue;
        }
        
        if (strcmp(cmd, "exit") == 0) {
            break;
        }
        
        int i = 0;
        args[i] = strtok(cmd, " ");
        while (args[i] != NULL && i < MAX_ARGS - 1) {
            i++;
            args[i] = strtok(NULL, " ");
        }
        args[i] = NULL;
        
        pid_t pid = fork();
        
        if (pid == 0) {
            execvp(args[0], args);
            perror("execvp failed");
            exit(1);
        } 
        else if (pid > 0) {
            wait(NULL);
        } 
        else {
            perror("fork failed");
        }
    }
    
    return 0;
}