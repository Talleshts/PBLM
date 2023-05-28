#include <stdio.h>
#include <stdlib.h>

#define MAX_TASKS 50

typedef struct {
    int task;
    int tempo;
} Tarefa;

void readInstance(const char* nomeArquivo, int* numeroTasks, int tempoTask[MAX_TASKS], int precedence[MAX_TASKS][MAX_TASKS]);
void createsolucaoInicial(int numeroTasks, int tempoTask[MAX_TASKS], Tarefa solucaoInicial[MAX_TASKS][MAX_TASKS]);
void applyLocalSearch(int numeroTasks, Tarefa tarefasPorTask[MAX_TASKS][MAX_TASKS]);
int calculaMakespan(int numeroTasks, Tarefa solucao[MAX_TASKS][MAX_TASKS]);
void writesolucao(const char* nomeArquivo, int numeroTasks, Tarefa solucao[MAX_TASKS][MAX_TASKS]);
void printsolucao(int numeroTasks, Tarefa solucao[MAX_TASKS][MAX_TASKS]);

int main() {
    int numeroTasks;
    int tempoTask[MAX_TASKS];
    int precedence[MAX_TASKS][MAX_TASKS];
    Tarefa solucaoInicial[MAX_TASKS][MAX_TASKS];

    readInstance("KILBRID.IN2", &numeroTasks, tempoTask, precedence);
    createsolucaoInicial(numeroTasks, tempoTask, solucaoInicial);

    applyLocalSearch(numeroTasks, solucaoInicial);

    calculaMakespan(numeroTasks, solucaoInicial);
    printsolucao(numeroTasks, solucaoInicial);
    writesolucao("output.txt", numeroTasks, solucaoInicial);

    return 0;
}

void readInstance(const char* nomeArquivo, int* numeroTasks, int tempoTask[MAX_TASKS], int precedence[MAX_TASKS][MAX_TASKS]) {
    // Implemente a leitura do arquivo de instância aqui
    // Extraia o número de tarefas, tempos de execução e precedências
    FILE* arquivo = fopen(nomeArquivo, "r");
    printf("%s\n", nomeArquivo);
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
    for (i = 0; i < *numeroTasks; i++) {
        for (j = 0; j < *numeroTasks; j++) {
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
        precedence[task1][task2] = 1; // Tarefa task1 precede tarefa task2
    }

    fclose(arquivo);
}

void createsolucaoInicial(int numeroTasks, int tempoTask[MAX_TASKS], Tarefa solucaoInicial[MAX_TASKS][MAX_TASKS]) {
    int i, j;

    // Ordenar as tarefas por tempo de execução em ordem decrescente
    for (i = 0; i < numeroTasks; i++) {
        for (j = i + 1; j < numeroTasks; j++) {
            if (tempoTask[j] > tempoTask[i]) {
                int temp = tempoTask[i];
                tempoTask[i] = tempoTask[j];
                tempoTask[j] = temp;
            }
        }
    }

    // Atribuir as tarefas ordenadas à solução inicial
    int task = 0;
    for (i = 0; i < numeroTasks; i++) {
        for (j = 0; j < tempoTask[i]; j++) {
            solucaoInicial[i][j].task = task;
            solucaoInicial[i][j].tempo = j + 1;
            task++;
        }
    }
}

void applyLocalSearch(int numeroTasks, Tarefa tarefasPorTask[MAX_TASKS][MAX_TASKS]) {
    int i, j;

    for (i = 0; i < numeroTasks; i++) {
        for (j = 0; j < numeroTasks; j++) {
            if (i != j && tarefasPorTask[i][0].tempo > tarefasPorTask[j][0].tempo) {
                int k = 0;
                while (tarefasPorTask[i][k].task != -1) {
                    int tempTask = tarefasPorTask[i][k].task;
                    int tempTempo = tarefasPorTask[i][k].tempo;
                    int l = 0;
                    while (tarefasPorTask[j][l].task != -1) {
                        if (tarefasPorTask[i][k].tempo <= tarefasPorTask[j][l].tempo) {
                            int m = 0;
                            while (tarefasPorTask[j][m].task != -1) {
                                if (tarefasPorTask[j][m].task == -1) {
                                    break;
                                }
                                m++;
                            }
                            int n = m;
                            while (n >= l) {
                                tarefasPorTask[j][n + 1].task = tarefasPorTask[j][n].task;
                                tarefasPorTask[j][n + 1].tempo = tarefasPorTask[j][n].tempo;
                                n--;
                            }
                            tarefasPorTask[j][l].task = tempTask;
                            tarefasPorTask[j][l].tempo = tempTempo;
                            break;
                        }
                        l++;
                    }
                    k++;
                }
            }
        }
    }
}

int calculaMakespan(int numeroTasks, Tarefa solucao[MAX_TASKS][MAX_TASKS]) {
    int makespan = 0;

    for (int i = 0; i < numeroTasks; i++) {
        int tempoFinal = solucao[i][0].tempo;
        for (int j = 0; j < numeroTasks && solucao[i][j].task != -1; j++) {
            if (solucao[i][j].tempo > tempoFinal) {
                tempoFinal = solucao[i][j].tempo;
            }
        }
        if (tempoFinal > makespan) {
            makespan = tempoFinal;
        }
    }

    return makespan;
}

void writesolucao(const char* nomeArquivo, int numeroTasks, Tarefa solucao[MAX_TASKS][MAX_TASKS]) {
    FILE* arquivo = fopen(nomeArquivo, "w");

    if (arquivo == NULL) {
        printf("Erro ao criar o arquivo de saída.\n");
        return;
    }

    for (int i = 0; i < numeroTasks; i++) {
        for (int j = 0; j < numeroTasks && solucao[i][j].task != -1; j++) {
            fprintf(arquivo, "%d,%d\n", solucao[i][j].task, solucao[i][j].tempo);
        }
    }

    fclose(arquivo);
}

void printsolucao(int numeroTasks, Tarefa solucao[MAX_TASKS][MAX_TASKS]) {
    for (int i = 0; i < numeroTasks; i++) {
        for (int j = 0; j < numeroTasks && solucao[i][j].task != -1; j++) {
            printf("(%d,%d) ", solucao[i][j].task, solucao[i][j].tempo);
        }
        printf("\n");
    }
}
