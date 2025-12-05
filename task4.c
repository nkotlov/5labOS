#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

int main(int argc, char *argv[]) {
    FILE *src, *dst;
    int c;
    struct stat st;
    
    if (argc != 3) {
        fprintf(stderr, "Использование: %s <исходный_файл> <целевой_файл>\n", argv[0]);
        return 1;
    }
    
    src = fopen(argv[1], "r");
    if (src == NULL) {
        perror("Ошибка открытия исходного файла");
        return 1;
    }
    
    if (stat(argv[1], &st) != 0) {
        perror("Ошибка получения прав доступа");
        fclose(src);
        return 1;
    }
    
    dst = fopen(argv[2], "w");
    if (dst == NULL) {
        perror("Ошибка открытия целевого файла");
        fclose(src);
        return 1;
    }
    
    while ((c = fgetc(src)) != EOF) {
        if (fputc(c, dst) == EOF) {
            perror("Ошибка записи в целевой файл");
            fclose(src);
            fclose(dst);
            return 1;
        }
    }
    
    if (ferror(src)) {
        perror("Ошибка чтения исходного файла");
        fclose(src);
        fclose(dst);
        return 1;
    }
    
    if (fclose(src) != 0) {
        perror("Ошибка закрытия исходного файла");
        fclose(dst);
        return 1;
    }
    
    if (fclose(dst) != 0) {
        perror("Ошибка закрытия целевого файла");
        return 1;
    }
    
    if (chmod(argv[2], st.st_mode) != 0) {
        perror("Ошибка копирования прав доступа");
        return 1;
    }
    
    printf("Файл '%s' успешно скопирован в '%s'\n", argv[1], argv[2]);
    printf("Права доступа скопированы: %o\n", st.st_mode & 0777);
    
    return 0;
}