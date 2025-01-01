#include <iostream>
#include <vector>

int main() {
    unsigned int n, t, pid, temp, acc = 0;
    scanf("%u %u", &n, &t);
    
    // Vetor de processos, com par (pid, tempo restante)
    std::vector<std::pair<unsigned int, int>> p;
    for (auto i = 0; i < n; i++) {
        scanf("%u %u", &pid, &temp);
        p.push_back(std::make_pair(pid, temp * 1000)); // Armazenando o tempo em milissegundos
    }
    
    unsigned int j = 0; // Índice do processo atual
    
    while (!p.empty()) { // Enquanto houver processos a serem executados
        // Reduz o tempo restante do processo atual
        p[j].second -= t;
        acc += t;

        // Verifica se o processo completou
        if (p[j].second <= 0) {
            acc += p[j].second; // Adiciona o tempo negativo restante
            printf("%u (%u)\n", p[j].first, acc); // Imprime o pid e o tempo acumulado

            // Remove o processo da lista
            p.erase(p.begin() + j);

            // Se removemos o último processo, volta para o início
            if (j == p.size()) {
                j = 0;
            }
        } else {
            // Se o processo não completou, apenas avança para o próximo
            if (++j == p.size()) {
                j = 0;
            }
        }
    }

    return 0;
}
