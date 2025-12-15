#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

void trata_sigchld(int sig) {
    int status;
    pid_t pid;

    while ((pid = waitpid(-1, &status, WNOHANG)) > 0) {
        printf("Filho %d terminou com status %d\n", pid, status);
    }
}

int main() {
    signal(SIGCHLD, trata_sigchld);

    for (int i = 0; i < 3; i++) {
        if (fork() == 0) {
            printf("Filho %d iniciado\n", getpid());
            sleep(10 + i * 5);
            exit(i);
        }
    }

    printf("Processo pai %d em execução...\n", getpid());

    while (1) {
        sleep(5);
        printf("Pai ainda rodando...\n");
    }

    return 0;
}
