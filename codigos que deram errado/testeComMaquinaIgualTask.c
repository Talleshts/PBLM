#include <stdio.h>
#include <stdlib.h>

#define MAX_TASKS 50
#define MAX_MACHINE 6

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
    int numeroMaquinas;

    readInstance("instancia.txt", &numeroTasks, tempoTask, precedence);
    numeroMaquinas = MAX_MACHINE;
    createsolucaoInicial(numeroTasks, tempoTask, solucaoInicial);

    applyLocalSearch(numeroTasks, solucaoInicial);

    int makespan = calculaMakespan(numeroTasks, solucaoInicial);

    printsolucao(numeroMaquinas, solucaoInicial);

    printf("FO: %d\n", makespan);

    writesolucao("output.txt", numeroTasks, solucaoInicial);

    return 0;
}

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
    for (i = 0; i < MAX_MACHINE && task < numeroTasks; i++) {  // Limit the loop to MAX_MACHINE and check if task < numeroTasks
        for (j = 0; j < tempoTask[i] && task < numeroTasks; j++) {  // Check if task < numeroTasks
            solucaoInicial[i][j].task = task;
            solucaoInicial[i][j].tempo = j + 1;
            task++;
        }
        solucaoInicial[i][j].task = -1; // Marca o fim da máquina
    }
}



void applyLocalSearch(int numeroTasks, Tarefa tarefasPorTask[MAX_TASKS][MAX_TASKS]) {
    int i, j, k, l, m;

    for (i = 0; i < numeroTasks; i++) {
        for (j = 0; j < numeroTasks; j++) {
            if (i != j && tarefasPorTask[i][0].tempo > tarefasPorTask[j][0].tempo) {
                k = 0;
                while (tarefasPorTask[i][k].task != -1) {
                    int tempTask = tarefasPorTask[i][k].task;
                    int tempTempo = tarefasPorTask[i][k].tempo;

                    l = 0;
                    while (tarefasPorTask[j][l].task != -1 && tarefasPorTask[i][k].tempo > tarefasPorTask[j][l].tempo) {
                        l++;
                    }

                    m = 0;
                    while (tarefasPorTask[j][m].task != -1) {
                        m++;
                    }

                    for (int n = m; n >= l; n--) {
                        tarefasPorTask[j][n + 1].task = tarefasPorTask[j][n].task;
                        tarefasPorTask[j][n + 1].tempo = tarefasPorTask[j][n].tempo;
                    }

                    tarefasPorTask[j][l].task = tempTask;
                    tarefasPorTask[j][l].tempo = tempTempo;
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
        printf("Máquina %d: ", i+1);
        for (int j = 0; j < numeroTasks && solucao[i][j].task != -1; j++) {
            printf("%d ", solucao[i][j].task);
        }
        printf("(tarefas atendidas)\n");
    }

    int makespan = calculaMakespan(numeroTasks, solucao);
    printf("FO: %d (soma dos custos das tarefas da máquina que possui o maior custo)\n", makespan);
}

