#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
int is_numeric(char* cuvant) {
    for (int i = 0; i <= strlen(cuvant) - 1; i++)
    {
        if (cuvant[i] >= '0' && cuvant[i] <= '9')
            continue;
        else
            return 0;
    }
    return 1;
}
int main(int argc, char* argv[]) {
    int fd[2], pid, p;
    char fisier[50], cuvant[50];
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
        int n;
        char cuvant[50], fisier[50];
        close(fd[0]);
        printf("Cititi N: ");
        scanf("%d", &n);
        printf("Cititi fisierul: ");
        scanf("%s", fisier);
        write(fd[1], &n, sizeof(int));
        write(fd[1], fisier, 50);
        sleep(1);
        FILE* file = fopen(fisier, "r");
        while (fscanf(file, "%s", cuvant) > 0) {
            write(fd[1], cuvant, 50);
        }
        fclose(file);
        char msg[50] = "stop";
        write(fd[1], msg, 50);
        close(fd[1]);
        exit(0);
    }
    else {// in parent
        char cuvant[50], fisier[50];
        char word[1010];
        int n;
        int contor = 0;
        char content[1000] = "\0";
        close(fd[1]);
        read(fd[0], &n, sizeof(int));
        read(fd[0], fisier, 50);
        printf("N: %d\n", n);
        printf("Fisier: %s\n", fisier);
        while (1) {
            read(fd[0], cuvant, 50);
            if (strcmp(cuvant, "stop") == 0) break;
            if (is_numeric(cuvant) == 1) contor++;
            strcat(content, cuvant);
            strcat(content, " ");
        }
        printf("Nr de numere din fisier este: %d\n", contor);
        if (contor % 2 == 0) {
            printf("Cititi un numar: ");
            scanf("%s", word);
            strcat(word, " ");
            strcat(word, content);
            FILE* file = fopen(fisier, "w");
            fwrite(word, 1, strlen(word) + 1, file);
            fclose(file);
        }
        wait(0);
        close(fd[0]);
    }
    return 0;
}