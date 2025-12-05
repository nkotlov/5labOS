#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>

int main(int argc, char *argv[]) {
    DIR *dir;
    struct dirent *entry;
    
    if (argc != 2) {
        fprintf(stderr, "Использование: %s <каталог>\n", argv[0]);
        return 1;
    }
    
    printf("Содержимое текущего каталога ('.'):\n");
    dir = opendir(".");
    if (dir == NULL) {
        perror("Ошибка открытия текущего каталога");
        return 1;
    }
    
    while ((entry = readdir(dir)) != NULL) {
        printf("%s\n", entry->d_name);
    }
    
    if (closedir(dir) != 0) {
        perror("Ошибка закрытия текущего каталога");
        return 1;
    }
    
    printf("\n");
    
    printf("Содержимое каталога '%s':\n", argv[1]);
    dir = opendir(argv[1]);
    if (dir == NULL) {
        perror("Ошибка открытия указанного каталога");
        return 1;
    }
    
    while ((entry = readdir(dir)) != NULL) {
        printf("%s\n", entry->d_name);
    }
    
    if (closedir(dir) != 0) {
        perror("Ошибка закрытия указанного каталога");
        return 1;
    }
    
    return 0;
}