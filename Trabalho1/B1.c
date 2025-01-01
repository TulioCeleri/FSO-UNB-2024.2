#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

volatile sig_atomic_t signal_count = 0;
pid_t zumbi_pid = -1;

void sigusr_handler(int sig) {
    // Incrementa a contagem de sinais recebidos
    signal_count++;

    if (signal_count == 1) {
        // Criar o processo zumbi quando receber o primeiro sinal
        zumbi_pid = fork();
        if (zumbi_pid == 0) {
            // Processo filho termina sem ser coletado pelo pai
            exit(0);
        }
    } else if (signal_count == 2) {
        // Matar o processo zumbi (recolher seu status) ao receber o segundo sinal
        if (zumbi_pid != -1) {
            waitpid(zumbi_pid, NULL, 0);  // Recolhe o processo zumbi
            zumbi_pid = -1;  // Zumbi foi destruído
        }
    } else if (signal_count == 3) {
        // Encerrar o programa com exit status 0
        exit(0);
    }
}

int main() {
    // Instalar tratadores de sinais
    signal(SIGUSR1, sigusr_handler);
    signal(SIGUSR2, sigusr_handler);

    // Esperar por sinais
    while (signal_count < 3) {
        pause();  // Pausa até receber um sinal
    }

    return 0;
}
