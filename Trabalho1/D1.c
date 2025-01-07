#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

// Variáveis globais
int count = 0; // Contador global
pthread_mutex_t mut; // Mutex para proteger o contador

// Estrutura de argumento para as threads
struct thread_arg {
    int vezes; // Número de vezes que cada thread deve chamar a função work
};

// Cabeçalho da função work
int work(int id);

// Função executada por cada thread
void *thread_func(void *arg) {
    struct thread_arg *args = (struct thread_arg *)arg;

    for (int i = 0; i < args->vezes; i++) {
        int id;

        // Bloqueia o mutex para obter um id único
        pthread_mutex_lock(&mut);
        id = count++;
        pthread_mutex_unlock(&mut);

        // Chama a função work com o id único
        work(id);
    }

    return NULL;
}
