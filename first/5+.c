#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>

#define BUFSIZE 500

int main(){
    int fd = open("file.txt", O_RDONLY);
    int terminal = open("/dev/tty", O_RDONLY | O_NDELAY);

    char c;
    long table[2][500];
    int i = 0, j = 0;
    char buf[BUFSIZE];
    
    while(read(fd, &c, 1)) {
        if (c == '\n') {
            j++;
            table[1][i] = j;
            table[0][i] = lseek(fd, 0L, SEEK_CUR);
            j = 0;
            i++;
        }
        else j++;
    }

    for (;;) {
        printf("You have 5 seconds to enter a line number: ");
        fflush(stdout);
        sleep(5);
        
        int bytes_read = read(terminal, buf, BUFSIZE - 1);
        if (bytes_read <= 0) {
            // Нет ввода - печатаем весь файл
            printf("\nTime's up! Printing entire file:\n");
            lseek(fd, 0L, SEEK_SET);
            int file_bytes;
            while ((file_bytes = read(fd, buf, BUFSIZE)) > 0) {
                write(STDOUT_FILENO, buf, file_bytes);
            }
            close(fd);
            close(terminal);
            exit(0);
        }
        else {
            // Есть ввод - обрабатываем только первое число
            buf[bytes_read] = '\0';
            
            int n = atoi(buf);
            
            if (n <= 0) {
                printf("Exiting...\n");
                close(fd);
                close(terminal);
                exit(0);
            }
            
            if (n > i) {
                printf("There are only %d rows!\n", i);
            }
            else {
                lseek(fd, table[0][n-1] - table[1][n-1], SEEK_SET);
                char line_buf[table[1][n-1] + 1];
                read(fd, line_buf, table[1][n-1]);
                line_buf[table[1][n-1]] = '\0';
                printf("%s", line_buf);
            }
        }
    }
}