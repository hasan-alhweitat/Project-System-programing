#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    pid_t pid;
    pid = fork();
    if (pid == 0) {
        execlp("gcc", "gcc", "part2.c", "-o", "part2.o", "-lcrypto", (char *)NULL);
        perror("execlp failed");
        exit(EXIT_FAILURE);
    } else if (pid > 0) {
        wait(NULL);
        execlp("./part2.o", "./part2.o", (char *)NULL);
        perror("execlp failed");
        exit(EXIT_FAILURE);
    }
    return 0;
}