#include <stdio.h>
#include <stdlib.h>

#define MAX_TASKS 50
#define MAX_MACHINES 11

typedef struct {
    int task;
    int tempo;
} Tarefa;

void readInstance(const char* nomeArquivo, int* numeroTasks, int tempoTask[MAX_TASKS], int precedence[MAX_TASKS][MAX_TASKS]);
void createsolucaoInicial(int numeroTasks, int numeroMaquinas, int tempoTask[MAX_TASKS], Tarefa solucaoInicial[MAX_MACHINES][MAX_TASKS]);
void applyLocalSearch(int numeroTasks, int numeroMaquinas, Tarefa tarefasPorTask[MAX_MACHINES][MAX_TASKS]);
int calculaMakespan(int numeroTasks, int numeroMaquinas, Tarefa solucao[MAX_MACHINES][MAX_TASKS]);
void writesolucao(const char* nomeArquivo, int numeroTasks, int numeroMaquinas, Tarefa solucao[MAX_MACHINES][MAX_TASKS]);
void printsolucao(int numeroTasks, int numeroMaquinas, Tarefa solucao[MAX_MACHINES][MAX_TASKS]);

int main() {
    int numeroTasks;
    int numeroMaquinas;
    int tempoTask[MAX_TASKS];
    int precedence[MAX_TASKS][MAX_TASKS];
    Tarefa solucaoInicial[MAX_MACHINES][MAX_TASKS];

    readInstance("instancia.txt", &numeroTasks, tempoTask, precedence);
    numeroMaquinas = 5;  // Defina o número de máquinas desejado aqui

    createsolucaoInicial(numeroTasks, numeroMaquinas, tempoTask, solucaoInicial);

    applyLocalSearch(numeroTasks, numeroMaquinas, solucaoInicial);

    calculaMakespan(numeroTasks, numeroMaquinas, solucaoInicial);
    printsolucao(numeroTasks, numeroMaquinas, solucaoInicial);
    writesolucao("output.txt", numeroTasks, numeroMaquinas, solucaoInicial);

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

void createsolucaoInicial(int numeroTasks, int numeroMaquinas, int tempoTask[MAX_TASKS], Tarefa solucaoInicial[MAX_MACHINES][MAX_TASKS]) {
    int maquinaAtual = 0;
    int taskIndex = 0;

    for (int i = 0; i < numeroTasks; i++) {
        int tempoExecucao = tempoTask[i];

        for (int j = 0; j < tempoExecucao; j++) {
            solucaoInicial[maquinaAtual][taskIndex].task = i;
            solucaoInicial[maquinaAtual][taskIndex].tempo = j + 1;
            taskIndex++;

            if (taskIndex == MAX_TASKS - 1) {
                solucaoInicial[maquinaAtual][taskIndex].task = -1;
                solucaoInicial[maquinaAtual][taskIndex].tempo = -1;

                maquinaAtual++;
                taskIndex = 0;
            }
        }
    }

    solucaoInicial[maquinaAtual][taskIndex].task = -1;
    solucaoInicial[maquinaAtual][taskIndex].tempo = -1;
}


void applyLocalSearch(int numeroTasks, int numeroMaquinas, Tarefa tarefasPorTask[MAX_MACHINES][MAX_TASKS]) {
    int i, j;

    for (i = 0; i < numeroMaquinas; i++) {
        for (j = 0; j < numeroMaquinas; j++) {
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
                                m++;
                            }
                            int n = m;
                            while (n > l) {
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


int calculaMakespan(int numeroTasks, int numeroMaquinas, Tarefa solucao[MAX_MACHINES][MAX_TASKS]) {
    int i, j;
    int tempoMaquina[MAX_MACHINES] = {0}; // Inicializa o tempo de cada máquina como zero

    for (i = 0; i < numeroMaquinas; i++) {
        for (j = 0; j < numeroTasks; j++) {
            if (solucao[i][j].task == -1) {
                break; // Fim da máquina
            }
            int task = solucao[i][j].task;
            int tempo = solucao[i][j].tempo;

            if (j == 0) {
                tempoMaquina[i] += tempo;
            } else {
                int taskAnterior = solucao[i][j - 1].task;
                int tempoAnterior = solucao[i][j - 1].tempo;

                if (tempoMaquina[i] <= tempoMaquina[taskAnterior]) {
                    tempoMaquina[i] = tempoMaquina[taskAnterior];
                }
                tempoMaquina[i] += tempo;
            }
        }
    }

    int makespan = tempoMaquina[0];
    for (i = 1; i < numeroMaquinas; i++) {
        if (tempoMaquina[i] > makespan) {
            makespan = tempoMaquina[i];
        }
    }

    return makespan;
}

void writesolucao(const char* nomeArquivo, int numeroTasks, int numeroMaquinas, Tarefa solucao[MAX_MACHINES][MAX_TASKS]) {
    // Implemente a escrita da solução no arquivo aqui
    FILE* arquivo = fopen(nomeArquivo, "w");
    if (arquivo == NULL) {
        printf("Erro ao criar o arquivo.\n");
        exit(1);
    }

    fprintf(arquivo, "%d\n", numeroTasks);
    fprintf(arquivo, "%d\n", numeroMaquinas);

    int i, j;
    for (i = 0; i < numeroMaquinas; i++) {
        for (j = 0; j < numeroTasks; j++) {
            if (solucao[i][j].task == -1) {
                break;
            }
            fprintf(arquivo, "%d %d\n", solucao[i][j].task, solucao[i][j].tempo);
        }
    }

    fclose(arquivo);
}

void printsolucao(int numeroTasks, int numeroMaquinas, Tarefa solucao[MAX_MACHINES][MAX_TASKS]) {
    for (int i = 0; i < numeroMaquinas; i++) {
        printf("Máquina %d: ", i + 1);
        for (int j = 0; j < numeroTasks && solucao[i][j].task != -1; j++) {
            printf("%d,%d ", solucao[i][j].task, solucao[i][j].tempo);
        }
        printf("(tarefas atendidas)\n");
    }

    int makespan = calculaMakespan(numeroTasks, numeroMaquinas, solucao);
    printf("FO: %d (soma dos custos das tarefas da máquina que possui o maior custo)\n", makespan);
}

