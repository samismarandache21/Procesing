#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
int main(int argc, char* argv[]) {
    char fisier1[50], fisier2[50];
    strcpy(fisier1, argv[1]);
    strcpy(fisier2, argv[2]);
    int pid, p, fd[2];
    p = pipe(fd);
    if (p == -1) {
        perror("pipe() error: ");
        exit(EXIT_FAILURE);
    }
    pid = fork();
    if (pid == -1) {
        perror("fork() error: ");
        exit(EXIT_FAILURE);
    }
    if (pid == 0) {//in child
        close(fd[0]);
        char cuvant[50];
        FILE* file = fopen(fisier1, "r");
        while (fscanf(file, "%s", cuvant) > 0) {
            write(fd[1], cuvant, 50);
        }
        fclose(file);
        char msg[50] = "stop";
        write(fd[1], msg, 50);
        close(fd[1]);
        exit(0);
    }
    else {//in parent
        close(fd[1]);
        char cuvant[50], cuvant2[50];
        char lista[50][50], content[50][50];
        int i = 0;
        int j = 0;
        char final[50] = "\0";
        while (1) {
            read(fd[0], cuvant, 50);
            if (strcmp(cuvant, "stop") == 0) break;
            strcpy(lista[i], cuvant);
            i++;
        }
        printf("%s %s\n", lista[0], lista[1]);
        printf("%s %s %s\n", lista[2], lista[3], lista[4]);
        printf("%s %s %s\n", lista[5], lista[6], lista[7]);
        FILE* file = fopen(fisier2, "r");
        while (fscanf(file, "%s", cuvant2) > 0) {
            strcpy(content[j], cuvant2);
            j++;
        }
        fclose(file);
        for (int ii = 0; ii <= i; ii++) {
            if (strcmp(lista[ii], "REPLACE") == 0) {
                for (int jj = 0; jj << j; jj++)
                    if (strcmp(content[jj], lista[ii + 1]) == 0)
                        strcpy(content[jj], lista[ii + 2]);
            }
        }
        for (int k = 0; k <= j; k++) {
            strcat(final, content[k]);
            printf("%s\n", content[k]);
        }
        wait(0);
        close(fd[0]);
    }
    return 0;
}