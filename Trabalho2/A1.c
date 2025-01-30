#include <stdio.h>
#include <stdlib.h>

/*
  Estrutura para armazenar informações de cada processo:
  - id: identificador (1-based)
  - instr: vetor de instruções (0 ou 1)
  - M: quantidade total de instruções
  - ptr: índice da próxima instrução a executar
  - finish_time: instante em que o processo terminou (em ms)
*/
typedef struct {
    int id;
    int *instr;
    int M;
    int ptr;
    long long finish_time;
} Process;


/* Nó da lista encadeada */
typedef struct Node {
    Process *proc;
    struct Node *next;
} Node;

/* Fila encadeada */
typedef struct {
    Node *front;
    Node *rear;
    int size;
} Queue;

/* Funções da fila encadeada */
Queue* createQueue() {
    Queue *q = (Queue*)malloc(sizeof(Queue));
    q->front = NULL;
    q->rear = NULL;
    q->size = 0;
    return q;
}

int isEmpty(Queue *q) {
    return (q->size == 0);
}

void enqueue(Queue *q, Process *p) {
    Node *newNode = (Node*)malloc(sizeof(Node));
    newNode->proc = p;
    newNode->next = NULL;

    if (isEmpty(q)) {
        q->front = newNode;
        q->rear = newNode;
    } else {
        q->rear->next = newNode;
        q->rear = newNode;
    }
    q->size++;
}

Process* dequeue(Queue *q) {
    if (isEmpty(q)) return NULL;
    Node *temp = q->front;
    Process *p = temp->proc;
    q->front = temp->next;
    free(temp);
    q->size--;
    if (q->size == 0) {
        q->rear = NULL;
    }
    return p;
}

int main() {
    int N;
    scanf("%d", &N);

    // Leitura dos M_i (quantidade de instruções de cada processo)
    int *M = (int*) malloc(N * sizeof(int));
    for (int i = 0; i < N; i++) {
        scanf("%d", &M[i]);
    }

    // Alocamos e lemos instruções de cada processo
    Process *processes = (Process*) malloc(N * sizeof(Process));
    for (int i = 0; i < N; i++) {
        processes[i].id = i + 1;
        processes[i].M = M[i];
        processes[i].ptr = 0;
        processes[i].finish_time = 0;

        processes[i].instr = (int*) malloc(M[i] * sizeof(int));
        for (int j = 0; j < M[i]; j++) {
            scanf("%d", &processes[i].instr[j]);
        }
    }

    // Criamos a fila encadeada e enfileiramos todos os processos
    Queue *q = createQueue();
    for (int i = 0; i < N; i++) {
        enqueue(q, &processes[i]);
    }

    long long tempo = 0;  // relógio global em ms

    // Enquanto a fila não estiver vazia, processamos
    int processosRestantes = N;

    while (!isEmpty(q)) {
        Process *atual = dequeue(q);

        // Executamos instruções a partir de 'ptr'
        while (atual->ptr < atual->M) {
            if (atual->instr[atual->ptr] == 0) {
                // Instrução não bloqueante: gasta 10ms
                tempo += 10;
                atual->ptr++;

                // Se concluiu todas as instruções:
                if (atual->ptr == atual->M) {
                    atual->finish_time = tempo;
                    processosRestantes--;
                }
            } else {
                // Instrução bloqueante (1): não gasta tempo agora,
                // converte para 0 e coloca o processo no fim da fila
                atual->instr[atual->ptr] = 0;
                // Reenfileirar e sair do while para escalonar o próximo processo
                enqueue(q, atual);
                break;
            }

            // Se já terminou as instruções, sai do while
            if (atual->ptr == atual->M) {
                break;
            }
        }
        // Se o processo terminou (ptr == M), não reenfileira
    }

    // Ordenar os processos por tempo de término (finish_time)
    Process *sorted = (Process*) malloc(N * sizeof(Process));
    for (int i = 0; i < N; i++) {
        sorted[i] = processes[i];
    }

    // Bubble sort simples (poderia usar qsort)
    for (int i = 0; i < N - 1; i++) {
        for (int j = 0; j < N - 1 - i; j++) {
            if (sorted[j].finish_time > sorted[j+1].finish_time) {
                Process temp = sorted[j];
                sorted[j] = sorted[j+1];
                sorted[j+1] = temp;
            }
        }
    }

    // Imprimir
    for (int i = 0; i < N; i++) {
        printf("%d(%lld)\n", sorted[i].id, sorted[i].finish_time);
    }

    // Liberar memória
    for (int i = 0; i < N; i++) {
        free(processes[i].instr);
    }
    free(processes);
    free(M);
    free(sorted);
    // Liberar nós da fila (se ainda restasse algo)
    free(q);

    return 0;
}
