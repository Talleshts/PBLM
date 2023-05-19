#include <stdio.h>
#include <stdlib.h>

#define MAX_TASKS 100

typedef struct {
    int task;
    int machine;
} Tarefa;

void readInstance(const char* nomeArquivo, int* numeroTasks, int tempoTask[MAX_TASKS], int* precedence[MAX_TASKS]) {
    // Implemente a leitura do arquivo de instância aqui
    // Extraia o número de tarefas, tempos de execução e precedências
    FILE* arquivo = fopen("KILBRID.IN2", "r");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        exit(1);
    }

    fscanf(arquivo, "%d", numeroTasks); // Lê o número de tarefas

    // Lê os tempos de execução das tarefas
    for (int i = 0; i < *numeroTasks; i++) {
        fscanf(arquivo, "%d", &tempoTask[i]);
    }

    // Lê as relações de precedência
    int i = 0;
    while (1) {
        int task1, task2;
        fscanf(arquivo, "%d,%d", &task1, &task2);
        if (task1 == -1 && task2 == -1) {
            break; // Marca de fim da instância
        }
        // Cria um array dinâmico para armazenar as precedências da tarefa i
        precedence[i] = malloc((*numeroTasks) * sizeof(int));
        precedence[i][task2] = 1; // Tarefa task1 precede tarefa task2
        i++;
    }

    fclose(arquivo);
}

void createsolucaoInicial(int numeroTasks, int tempoTask[MAX_TASKS], Tarefa solucao[MAX_TASKS]) {
    // Implemente a criação da solução inicial aqui
}

int calculaMakespan(int numeroMaquinas, Tarefa solucao[MAX_TASKS]) {
    // Implemente o cálculo do makespan aqui
}

void writesolucao(const char* nomeArquivo, int numeroMaquinas, Tarefa solucao[MAX_TASKS]) {
    // Implemente a escrita da solução em arquivo aqui
}

void printsolucao(int numeroMaquinas, Tarefa solucao[MAX_TASKS]) {
    // Implemente a impressão da solução na tela aqui
}

void applyLocalSearch(int numeroMaquinas, Tarefa solucao[MAX_TASKS]) {
    // Implemente a busca local aqui
}

int main() {
    int numeroTasks;
    int tempoTask[MAX_TASKS];
    int *precedence[MAX_TASKS];

    readInstance("KILBRID.IN2", &numeroTasks, tempoTask, precedence);

    Tarefa solucaoInicial[MAX_TASKS];
    createsolucaoInicial(numeroTasks, tempoTask, solucaoInicial);

    //applyLocalSearch(numeroMaquinas, solucaoInicial);

    //int bestMakespan = calculaMakespan(numeroMaquinas, solucaoInicial);

    //writesolucao("output.txt", numeroMaquinas, solucaoInicial);

    //printsolucao(numeroMaquinas, solucaoInicial);

    return 0;
}
