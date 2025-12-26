#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <time.h>

int main() {
    pid_t pid1 = fork();
    
    if (pid1 == 0) {
        struct timeval tv;
        gettimeofday(&tv, NULL);
        struct tm *tm = localtime(&tv.tv_sec);
        printf("Дочерний 1: PID=%d, PPID=%d, Время=%02d:%02d:%02d:%03d\n",
               getpid(), getppid(),
               tm->tm_hour, tm->tm_min, tm->tm_sec,
               (int)(tv.tv_usec / 1000));
        sleep(2);
        exit(0);
    }
    
    pid_t pid2 = fork();
    
    if (pid2 == 0) {
        struct timeval tv;
        gettimeofday(&tv, NULL);
        struct tm *tm = localtime(&tv.tv_sec);
        printf("Дочерний 2: PID=%d, PPID=%d, Время=%02d:%02d:%02d:%03d\n",
               getpid(), getppid(),
               tm->tm_hour, tm->tm_min, tm->tm_sec,
               (int)(tv.tv_usec / 1000));
        sleep(2);
        exit(0);
    }
    
    if (pid1 > 0 && pid2 > 0) {
        struct timeval tv;
        gettimeofday(&tv, NULL);
        struct tm *tm = localtime(&tv.tv_sec);
        printf("Родитель: PID=%d, PPID=%d, Время=%02d:%02d:%02d:%03d\n",
               getpid(), getppid(),
               tm->tm_hour, tm->tm_min, tm->tm_sec,
               (int)(tv.tv_usec / 1000));
        
        system("ps -x");
        
        wait(NULL);
        wait(NULL);
    }
    
    return 0;
}