#include <stdio.h>
#include <stdlib.h>

#define MAX_TASKS 50

typedef struct {
    int task;
    int tempo;
} Tarefa;

void readInstance(const char* nomeArquivo, int* numeroTasks, int tempoTask[MAX_TASKS], int precedence[MAX_TASKS][MAX_TASKS]) {
    // Implemente a leitura do arquivo de instância aqui
    // Extraia o número de tarefas, tempos de execução e precedências
    FILE* arquivo = fopen(nomeArquivo, "r");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        exit(1);
    }

    fscanf(arquivo, "%d", numeroTasks); // Lê o número de tarefas

	int i, j;
    // Lê os tempos de execução das tarefas
    for (i = 0; i < *numeroTasks; i++) {
        fscanf(arquivo, "%d", &tempoTask[i]);
    }

    // Inicializa a matriz de precedências com zeros
    for (int i = 0; i < *numeroTasks; i++) {
        for (int j = 0; j < *numeroTasks; j++) {
            precedence[i][j] = 0;
        }
    }

    // Lê as relações de precedência entre as tarefas
    while (1) {
        int task1, task2;
        fscanf(arquivo, "%d,%d", &task1, &task2);
        if (task1 == -1 && task2 == -1) {
            break; // Marca de fim da instância
        }
        // Cria um array dinâmico para armazenar as precedências da tarefa i
        precedence[task1][task2] = 1; // Tarefa task1 precede tarefa task2
    }

    fclose(arquivo);
}

void createsolucaoInicial(int numeroTasks, int tempoTask[MAX_TASKS], Tarefa solucao[MAX_TASKS]) {
    int i,j;

    // Ordenar as tarefas por tempo de execução em ordem decrescente
    for (i = 0; i < numeroTasks; i++) {
        solucao[i].task = i;
        solucao[i].tempo = tempoTask[i];
    }

    for (i = 0; i < numeroTasks - 1; i++) {
        for (j = i + 1; j < numeroTasks; j++) {
            if (solucao[j].tempo > solucao[i].tempo) {
                Tarefa temp = solucao[i];
                solucao[i] = solucao[j];
                solucao[j] = temp;
            }
        }
    }

    int taskAtual = 0;

    for (i = 0; i < numeroTasks; i++) {
        solucao[i].task = taskAtual;
        taskAtual = (taskAtual + 1) % numeroTasks; // Alternar para a próxima máquina
    }
}

void printsolucao(int numeroTasks, Tarefa solucao[MAX_TASKS]) {
    printf("Solucao:\n");
    int i;
    for (i = 0; i < MAX_TASKS; i++) {
        if (solucao[i].task != -1) {
            printf("Máquina %d: ", solucao[i].task + 1);
            printf("%d", solucao[i].task);
            int j = i + 1;
            while (j < MAX_TASKS && solucao[j].task == solucao[i].task) {
                printf(", %d", solucao[j].task);
                j++;
            }
            printf("\n");
        }
    }
}

int main() {
    int numeroTasks;
    int tempoTask[MAX_TASKS];
    int precedence[MAX_TASKS][MAX_TASKS];

    readInstance("KILBRID.IN2", &numeroTasks, tempoTask, precedence);
    Tarefa solucaoInicial[MAX_TASKS];
    createsolucaoInicial(numeroTasks, tempoTask, solucaoInicial);

    printsolucao(numeroTasks, solucaoInicial);

    return 0;
}
