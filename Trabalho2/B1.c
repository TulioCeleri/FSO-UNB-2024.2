#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// Para filas, podemos usar um array circular. 
// Mas para simplicidade aqui, usaremos uma lista encadeada 
// ou algo que não limite N (desde que N e Q caibam na memória).

// Estrutura nó da fila
typedef struct Node {
    int page;
    struct Node* next;
} Node;

// Fila encadeada
typedef struct {
    Node* front; // frente (remove daqui)
    Node* rear;  // fim (insere aqui)
    int size;    // quantos elementos na fila
} Queue;

// Cria uma fila vazia
Queue* createQueue() {
    Queue* q = (Queue*)malloc(sizeof(Queue));
    q->front = NULL;
    q->rear = NULL;
    q->size = 0;
    return q;
}

// Verifica se está vazia
int isEmpty(Queue* q) {
    return (q->size == 0);
}

// Enfileira
void enqueue(Queue* q, int page) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->page = page;
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

// Desenfileira (retorna a página mais antiga)
int dequeue(Queue* q) {
    if (isEmpty(q)) {
        return -1; // ou erro
    }
    Node* temp = q->front;
    int oldPage = temp->page;
    q->front = temp->next;
    free(temp);
    q->size--;
    if (q->size == 0) {
        q->rear = NULL;
    }
    return oldPage;
}

int main() {
    int Q, N;
    scanf("%d", &Q); // quantidade de quadros
    scanf("%d", &N); // quantidade de referências

    // Alocamos um array de flags para saber se 
    // a página x está ou não na memória.
    // Páginas podem ser até 10^6, então 
    // faremos presente[10^6+1].
    bool *presente = (bool*) calloc(1000001, sizeof(bool));
    // Inicialmente, tudo é false (0).

    // Criamos a fila para armazenar as páginas que estão 
    // efetivamente na memória e sua ordem de entrada.
    Queue* frames = createQueue();

    long long page_faults = 0; // contador de page faults

    for(int i = 0; i < N; i++) {
        int pagina;
        scanf("%d", &pagina);

        if (!presente[pagina]) {
            // Page fault
            page_faults++;

            // Se a fila ainda não está cheia (size < Q), 
            // apenas enfileira
            if (frames->size < Q) {
                enqueue(frames, pagina);
                presente[pagina] = true;
            } else {
                // A fila está cheia, então retira a página mais antiga
                int oldPage = dequeue(frames);
                presente[oldPage] = false;

                // E coloca a nova página
                enqueue(frames, pagina);
                presente[pagina] = true;
            }
        }
        // se presente[pagina] == true, não faz nada (sem page fault)
    }

    printf("%lld\n", page_faults);

    // limpeza
    free(presente);
    // Libertar nós da fila
    while (!isEmpty(frames)) {
        dequeue(frames);
    }
    free(frames);

    return 0;
}
