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

    readInstance("input.txt", &numeroTasks, tempoTask, precedence);

    Tarefa solucaoInicial[MAX_TASKS];
    createsolucaoInicial(numeroTasks, tempoTask, solucaoInicial);

    applyLocalSearch(numeroMaquinas, solucaoInicial);

    int bestMakespan = calculaMakespan(numeroMaquinas, solucaoInicial);

    writesolucao("output.txt", numeroMaquinas, solucaoInicial);

    printsolucao(numeroMaquinas, solucaoInicial);

    return 0;
}
