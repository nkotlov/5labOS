#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    FILE *file;
    int c;
    int n;
    int lines = 0;
    int chars_in_line = 0;
    
    if (argc != 3) {
        fprintf(stderr, "Использование: %s <файл> <N>\n", argv[0]);
        return 1;
    }
    
    n = atoi(argv[2]);
    if (n < 0) {
        fprintf(stderr, "Ошибка: N не может быть отрицательным\n");
        return 1;
    }
    
    file = fopen(argv[1], "r");
    if (file == NULL) {
        perror("Ошибка открытия файла");
        return 1;
    }
    
    while ((c = fgetc(file)) != EOF) {
        fputc(c, stdout);
        chars_in_line++;
        
        if (c == '\n') {
            lines++;
            chars_in_line = 0;
            
            if (n > 0 && lines % n == 0) {
                printf("[Нажмите любую клавишу для продолжения...]");
                fgetc(stdin);
            }
        }
    }
    
    if (ferror(file)) {
        perror("Ошибка чтения файла");
        fclose(file);
        return 1;
    }
    
    if (fclose(file) != 0) {
        perror("Ошибка закрытия файла");
        return 1;
    }
    
    if (chars_in_line > 0) {
        lines++;
    }
    
    printf("\nВсего строк: %d\n", lines);
    return 0;
}