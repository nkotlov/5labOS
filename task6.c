#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
#include <time.h>

void search_file(const char *dirpath, const char *filename, int *dirs_count, int *files_count) {
    DIR *dir;
    struct dirent *entry;
    struct stat st;
    char path[4096];
    
    dir = opendir(dirpath);
    if (dir == NULL) {
        return;
    }
    
    (*dirs_count)++;
    
    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }
        
        snprintf(path, sizeof(path), "%s/%s", dirpath, entry->d_name);
        
        if (lstat(path, &st) != 0) {
            continue;
        }
        
        if (S_ISDIR(st.st_mode)) {
            search_file(path, filename, dirs_count, files_count);
        } else if (S_ISREG(st.st_mode)) {
            (*files_count)++;
            
            if (strcmp(entry->d_name, filename) == 0) {
                printf("Путь: %s\n", path);
                printf("Имя: %s\n", entry->d_name);
                printf("Размер: %ld байт\n", st.st_size);
                printf("Права: %o\n", st.st_mode & 0777);
                printf("Inode: %lu\n", (unsigned long)st.st_ino);
                printf("Дата изменения: %s", ctime(&st.st_mtime));
                printf("---\n");
            }
        }
    }
    
    closedir(dir);
}

int main(int argc, char *argv[]) {
    int dirs_count = 0;
    int files_count = 0;
    struct stat st;
    
    if (argc != 3) {
        fprintf(stderr, "Использование: %s <каталог> <имя_файла>\n", argv[0]);
        return 1;
    }
    
    if (stat(argv[1], &st) != 0 || !S_ISDIR(st.st_mode)) {
        fprintf(stderr, "'%s' не является каталогом\n", argv[1]);
        return 1;
    }
    
    search_file(argv[1], argv[2], &dirs_count, &files_count);
    
    if (files_count == 0) {
        printf("Файл '%s' не найден в каталоге '%s'\n", argv[2], argv[1]);
    }
    
    printf("Просмотрено каталогов: %d\n", dirs_count);
    printf("Просмотрено файлов: %d\n", files_count);
    
    return 0;
}