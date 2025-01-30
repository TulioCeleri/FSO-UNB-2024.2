#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// Estrutura de nó da lista duplamente encadeada
typedef struct DListNode {
    int page;
    struct DListNode* prev;
    struct DListNode* next;
} DListNode;

// Ponteiro para a cabeça e cauda da lista
typedef struct {
    DListNode* head;  // mais recentemente usado
    DListNode* tail;  // menos recentemente usado
    int size;         // quantos nós/páginas estão na lista
} DList;

// Funções para manipular a lista duplamente encadeada

// Cria uma lista vazia
DList* createDList() {
    DList* list = (DList*)malloc(sizeof(DList));
    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
    return list;
}

// Insere nó na frente (head) da lista (nó vira "mais recentemente usado")
void insertFront(DList* list, DListNode* node) {
    node->prev = NULL;
    node->next = list->head;

    if (list->head) {
        list->head->prev = node;
    }
    list->head = node;
    if (list->tail == NULL) {
        // lista estava vazia
        list->tail = node;
    }
    list->size++;
}

// Remove um nó arbitrário da lista (em O(1), pois temos ponteiro direto para ele)
void removeNode(DList* list, DListNode* node) {
    if (!node) return;

    if (node->prev) {
        node->prev->next = node->next;
    } else {
        // se não tem prev, então node é o head
        list->head = node->next;
    }

    if (node->next) {
        node->next->prev = node->prev;
    } else {
        // se não tem next, então node é o tail
        list->tail = node->prev;
    }
    list->size--;
}

// Remove o nó do final da lista (tail) e retorna o ponteiro para ele
// (LRU = o menos recentemente usado está no tail)
DListNode* removeTail(DList* list) {
    if (list->tail == NULL) {
        return NULL;
    }
    DListNode* oldTail = list->tail;
    removeNode(list, oldTail);
    return oldTail;
}

// Move um nó já existente para a frente da lista
// (significa que essa página se tornou a mais recentemente usada)
void moveToFront(DList* list, DListNode* node) {
    // Primeiro removemos da posição atual
    removeNode(list, node);
    // Depois inserimos na frente
    insertFront(list, node);
}

int main() {
    int Q, N;
    scanf("%d", &Q); // quantidade de quadros
    scanf("%d", &N); // quantidade de referências

    // Teremos pages até 10^6,
    // então um array de ponteiros de tamanho (10^6 + 1) é aceitável (~ 8 MB se 64 bits, ou ~4 MB se 32 bits).
    // Se for muito, é possível usar uma estrutura de mapa hash, mas nesse caso costuma funcionar.

    DListNode** presence = (DListNode**) calloc(1000001, sizeof(DListNode*));
    // presence[page] = ponteiro para o nó dessa página na lista, ou NULL se não estiver em memória

    DList* list = createDList(); // lista para manter a ordem LRU
    long long page_faults = 0;

    // Lemos cada referência e simulamos LRU
    for(int i = 0; i < N; i++) {
        int page;
        scanf("%d", &page);

        // Verifica se já está na memória
        if (presence[page] != NULL) {
            // Já está na lista, move para a frente (mais recentemente usado)
            moveToFront(list, presence[page]);

        } else {
            // Não está na memória => page fault
            page_faults++;

            // Se ainda tem espaço nos quadros
            if (list->size < Q) {
                // Cria nó e insere na frente
                DListNode* newNode = (DListNode*)malloc(sizeof(DListNode));
                newNode->page = page;
                newNode->prev = NULL;
                newNode->next = NULL;

                insertFront(list, newNode);
                presence[page] = newNode;

            } else {
                // Memória cheia: remove LRU do tail
                DListNode* oldTail = removeTail(list);
                int oldPage = oldTail->page;

                // Marca que oldPage não está mais na memória
                presence[oldPage] = NULL;
                // libera o nó antigo
                free(oldTail);

                // Cria novo nó para a página atual
                DListNode* newNode = (DListNode*)malloc(sizeof(DListNode));
                newNode->page = page;
                newNode->prev = NULL;
                newNode->next = NULL;

                insertFront(list, newNode);
                presence[page] = newNode;
            }
        }
    }

    // Imprimir o número de page faults
    printf("%lld\n", page_faults);

    // Limpeza de memória
    // Vamos remover todos nós da lista
    while (list->head != NULL) {
        DListNode* temp = list->head;
        removeNode(list, temp);
        free(temp);
    }
    free(list);
    free(presence);

    return 0;
}
