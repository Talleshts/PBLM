#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_TASKS 50
#define MAX_MACHINES 13


typedef struct {
    int task;
    int tempo;
} Tarefa;

void createsolucaoInicial(Tarefa *solucao) {
    int i, j;
    int taskAtual;
    for (i = 0; i < MAX_TASKS; i++) {
        solucao[i].task = -1;
        solucao[i].tempo = 0;
    }

    for (i = 0; i < MAX_TASKS; i++) {
        taskAtual = rand() % MAX_TASKS;

        while (solucao[taskAtual].task != -1) {
            taskAtual = (taskAtual + 1) % MAX_TASKS;
        }

        solucao[i].task = taskAtual;
        solucao[i].tempo = rand() % 10 + 1; // Random tempo between 1 and 10

        for (j = 0; j < i; j++) {
            if (solucao[j].tempo > solucao[i].tempo) {
                Tarefa temp = solucao[i];
                solucao[i] = solucao[j];
                solucao[j] = temp;
            }
        }
    }
}

void calculaMakespan(Tarefa *solucao, int *makespan) {
    int i;
    for (i = 0; i < MAX_TASKS; i++) {
        makespan[i] = 0;
    }

    for (i = 0; i < MAX_TASKS; i++) {
        int task = solucao[i].task;
        makespan[task] += solucao[i].tempo;
    }
}

void writesolucao(Tarefa *solucao, FILE *arquivo) {
    int i, j;
    for (i = 0; i < MAX_TASKS; i++) {
        if (solucao[i].task != -1) {
            fprintf(arquivo, "Máquina %d: ", solucao[i].task + 1);
            fprintf(arquivo, "%d", solucao[i].task);

            j = i + 1;
            while (j < MAX_TASKS && solucao[j].task == solucao[i].task) {
                fprintf(arquivo, ", %d", solucao[j].task);
                j++;
            }
            fprintf(arquivo, "\n");
        }
    }
}

void printsolucao(Tarefa *solucao) {
    int i, j;
    for (i = 0; i < MAX_TASKS; i++) {
        if (solucao[i].task != -1) {
            printf("Máquina %d: ", solucao[i].task + 1);
            printf("%d", solucao[i].task);

            j = i + 1;
            while (j < MAX_TASKS && solucao[j].task == solucao[i].task) {
                printf(", %d", solucao[j].task);
                j++;
            }
            printf("\n");
        }
    }
}

int main() {
    srand(time(NULL));
    Tarefa solucao[MAX_TASKS];
    int makespan[MAX_MACHINES] = {0};  // Inicializa makespan com 0 para cada máquina
    FILE *arquivo = fopen("solucao.txt", "w");

    if (arquivo == NULL) {
        printf("Erro ao criar arquivo.\n");
        return 1;
    }

    createsolucaoInicial(solucao);
    calculaMakespan(solucao, makespan);

    writesolucao(solucao, arquivo);
    printsolucao(solucao);

    fclose(arquivo);

    // Calculate the objective function (FO)
    int fo = 0;
    for (int i = 0; i < MAX_MACHINES; i++) {
        if (makespan[i] > fo) {
            fo = makespan[i];
        }
    }

    printf("Função objetivo (FO): %d\n", fo);

    return 0;
}