#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <unistd.h>

sig_atomic_t child_exit_status;

void clean_up_child_process(int signal_number) {
    int status;
    while (waitpid(-1, &status, WNOHANG) > 0) {
        child_exit_status = status;
    }
}

int main() {
    struct sigaction sigchld_action;
    memset(&sigchld_action, 0, sizeof(sigchld_action));
    sigchld_action.sa_handler = clean_up_child_process;
    sigaction(SIGCHLD, &sigchld_action, NULL);

    // Criação de processos filhos
    for (int i = 0; i < 5; i++) {
        pid_t child_pid = fork();
        if (child_pid == 0) {
            // Filho
            sleep(10 + i); // Viver por 10 segundos mais i segundos
            exit(0);
        }
    }
    
    // Manter o pai executando
    while (1) {
        sleep(1);
    }
    
    return 0;
}
