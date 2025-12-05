#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    FILE *file;
    int c;
    
    if (argc != 2) {
        fprintf(stderr, "Использование: %s <имя_файла>\n", argv[0]);
        return 1;
    }

    file = fopen(argv[1], "w");
    if (file == NULL) {
        perror("Ошибка открытия файла");
        return 1;
    }
    
    printf("Введите текст. Для завершения нажмите Ctrl+F:\n");
    
    while ((c = getc(stdin)) != EOF) {
        if (c == 6) {
            printf("\nЗавершение ввода...\n");
            break;
        }
        
        if (putc(c, file) == EOF) {
            perror("Ошибка записи в файл");
            fclose(file);
            return 1;
        }
    }
    
    if (fclose(file) != 0) {
        perror("Ошибка закрытия файла");
        return 1;
    }
    
    printf("Данные успешно записаны в файл '%s'\n", argv[1]);
    return 0;
}