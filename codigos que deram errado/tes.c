#include <stdio.h>
#include <stdlib.h>

#define MAX_TASKS 50
#define MAX_MACHINE 12

typedef struct {
    int task;
    int tempo;
} Tarefa;

void readInstance(const char* nomeArquivo, int* numeroTasks, int tempoTask[MAX_TASKS], int precedence[MAX_TASKS][MAX_TASKS]);
void createInitialSolution(int numeroTasks, int tempoTask[MAX_TASKS], Tarefa solucaoInicial[MAX_TASKS][MAX_TASKS]);
void applyLocalSearch(int numeroTasks, Tarefa solucaoInicial[MAX_TASKS][MAX_TASKS]);
int calculaMakespan(int numeroTasks, Tarefa solucao[MAX_TASKS][MAX_TASKS]);
void writeSolution(const char* nomeArquivo, int numeroTasks, Tarefa solucao[MAX_TASKS][MAX_TASKS]);
void printSolution(int numeroTasks, Tarefa solucao[MAX_TASKS][MAX_TASKS]);


int main() {
    int numeroTasks;
    int tempoTask[MAX_TASKS];
    int precedence[MAX_TASKS][MAX_TASKS];
    Tarefa solucaoInicial[MAX_TASKS][MAX_TASKS];
    int numeroMaquinas;

    readInstance("instancia.txt", &numeroTasks, tempoTask, precedence);
    numeroMaquinas = MAX_MACHINE; // Update to MAX_MACHINE

    createInitialSolution(numeroTasks, tempoTask, solucaoInicial);

    applyLocalSearch(numeroTasks, solucaoInicial);

    int makespan = calculaMakespan(numeroTasks, solucaoInicial);

    printSolution(numeroMaquinas, solucaoInicial);

    printf("FO: %d\n", makespan);

    writeSolution("output.txt", numeroTasks, solucaoInicial);

    return 0;
}

void readInstance(const char* nomeArquivo, int* numeroTasks, int tempoTask[MAX_TASKS], int precedence[MAX_TASKS][MAX_TASKS]) {
    // Implement file reading logic here
    // Extract number of tasks, task times, and precedences
    FILE* arquivo = fopen(nomeArquivo, "r");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        exit(1);
    }

    fscanf(arquivo, "%d", numeroTasks); // Read the number of tasks

    for (int i = 0; i < *numeroTasks; i++) {
        fscanf(arquivo, "%d", &tempoTask[i]); // Read task times
    }

    // Initialize precedence matrix with zeros
    for (int i = 0; i < *numeroTasks; i++) {
        for (int j = 0; j < *numeroTasks; j++) {
            precedence[i][j] = 0;
        }
    }

    // Read task precedences
    while (1) {
        int task1, task2;
        fscanf(arquivo, "%d,%d", &task1, &task2);
        if (task1 == -1 && task2 == -1) {
            break; // End of instance marker
        }
        precedence[task1][task2] = 1; // Task task1 precedes task task2
    }

    fclose(arquivo);
}

void createInitialSolution(int numeroTasks, int tempoTask[MAX_TASKS], Tarefa solucaoInicial[MAX_TASKS][MAX_TASKS]) {
    // Sort tasks by execution time in descending order
    for (int i = 0; i < numeroTasks; i++) {
        for (int j = i + 1; j < numeroTasks; j++) {
            if (tempoTask[j] > tempoTask[i]) {
                int temp = tempoTask[i];
                tempoTask[i] = tempoTask[j];
                tempoTask[j] = temp;
            }
        }
    }

    // Assign the sorted tasks to the initial solution
    int task = 0;
    for (int i = 0; i < numeroTasks; i++) {
        for (int j = 0; j < tempoTask[i]; j++) {
            solucaoInicial[i][j].task = task;
            solucaoInicial[i][j].tempo = j + 1;
            task++;
        }
        solucaoInicial[i][tempoTask[i]].task = -1; // Mark the end of the machine
    }
}

void applyLocalSearch(int numeroTasks, Tarefa solucaoInicial[MAX_TASKS][MAX_TASKS]) {
    int swapped;

    do {
        swapped = 0;

        for (int i = 0; i < numeroTasks; i++) {
            for (int j = 0; j < numeroTasks; j++) {
                if (i != j && solucaoInicial[i][0].tempo > solucaoInicial[j][0].tempo) {
                    for (int k = 0; solucaoInicial[i][k].task != -1; k++) {
                        if (solucaoInicial[i][k].tempo > solucaoInicial[j][0].tempo) {
                            int tempTask = solucaoInicial[i][k].task;
                            int tempTempo = solucaoInicial[i][k].tempo;

                            int l = 0;
                            while (solucaoInicial[j][l].task != -1 && solucaoInicial[i][k].tempo > solucaoInicial[j][l].tempo) {
                                l++;
                            }

                            int m = 0;
                            while (solucaoInicial[j][m].task != -1) {
                                m++;
                            }

                            for (int n = m; n >= l; n--) {
                                solucaoInicial[j][n + 1].task = solucaoInicial[j][n].task;
                                solucaoInicial[j][n + 1].tempo = solucaoInicial[j][n].tempo;
                            }

                            solucaoInicial[j][l].task = tempTask;
                            solucaoInicial[j][l].tempo = tempTempo;
                            swapped = 1;
                            break;
                        }
                    }
                }
            }
        }
    } while (swapped);
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

void writeSolution(const char* nomeArquivo, int numeroTasks, Tarefa solucao[MAX_TASKS][MAX_TASKS]) {
    FILE* arquivo = fopen(nomeArquivo, "w");

    if (arquivo == NULL) {
        printf("Erro ao criar o arquivo de saída.\n");
        return;
    }

    for (int i = 0; i < numeroTasks; i++) {
        for (int j = 0; solucao[i][j].task != -1; j++) {
            fprintf(arquivo, "%d,%d ", solucao[i][j].task, solucao[i][j].tempo);
        }
        fprintf(arquivo, "\n");
    }

    fclose(arquivo);
}

void printSolution(int numeroTasks, Tarefa solucao[MAX_TASKS][MAX_TASKS]) {
    for (int i = 0; i < numeroTasks; i++) {
        printf("Máquina %d: ", i + 1);
        for (int j = 0; solucao[i][j].task != -1; j++) {
            printf("%d ", solucao[i][j].task);
        }
        printf("\n");
    }
}

