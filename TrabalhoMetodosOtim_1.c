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

<<<<<<< HEAD
    // Lê as relações de precedência
    int i = 0;

    for (i = 0; i < MAX_TASKS; i++) {
        for(int j = 0; j < MAX_TASKS; j++){
            precedence[i][j] = 0;
        }
        //precedence[i] = malloc(MAX_TASKS * sizeof(int));
=======
    // Inicializa a matriz de precedências com zeros
    for (int i = 0; i < *numeroTasks; i++) {
        for (int j = 0; j < *numeroTasks; j++) {
            precedence[i][j] = 0;
        }
>>>>>>> d2e9570800099159273407ae8a74b10ebd57404f
    }

    // Lê as relações de precedência entre as tarefas
    while (1) {
        int task1, task2;
        fscanf(arquivo, "%d,%d", &task1, &task2);
        if (task1 == -1 && task2 == -1) {
            break; // Marca de fim da instância
        }
        // Cria um array dinâmico para armazenar as precedências da tarefa i
        //precedence[i] = malloc((*numeroTasks) * sizeof(int));
        precedence[task1][task2] = 1; // Tarefa task1 precede tarefa task2
    }

    for (i = 0; i < MAX_TASKS; i++) {
        for(int j = 0; j < MAX_TASKS; j++){
            printf("|%d ", precedence[i][j]);
        }
        printf("\n");
    }

    fclose(arquivo);
}

/*
void createsolucaoInicial(int numeroTasks, int tempoTask[MAX_TASKS], Tarefa solucao[MAX_TASKS]) {
    // Implemente a criação da solução inicial aqui
    int i;

    // Ordenar as tarefas por tempo de execução em ordem decrescente
    for (i = 0; i < numeroTasks; i++) {
        solucao[i].task = i;
        solucao[i].tempo = tempoTask[i];
    }

    for (i = 0; i < numeroTasks - 1; i++) {
        int j;
        for (j = i + 1; j < numeroTasks; j++) {
            if (solucao[j].tempo > solucao[i].tempo) {
                Tarefa temp = solucao[i];
                solucao[i] = solucao[j];
                solucao[j] = temp;
            }
        }
    }

    // Alocar as tarefas sequencialmente nas máquinas
    int numtasks = 3; // Número de máquinas (definido manualmente neste exemplo)
    int taskAtual = 0;

    for (i = 0; i < numeroTasks; i++) {
        solucao[i].task = taskAtual;
        taskAtual = (taskAtual + 1) % numtasks; // Alternar para a próxima máquina
    }
}

int calculaMakespan(int numeroTasks, Tarefa solucao[MAX_TASKS]) {
    // Implemente o cálculo do makespan aqui
    int makespan[MAX_TASKS] = {0}; // Inicializa o makespan de cada máquina como 0

    // Calcula o task de conclusão de cada máquina
    int i;
    for (i = 0; i < MAX_TASKS; i++) {
        int task = solucao[i].task;
        makespan[task] += solucao[i].task;
    }

    // Encontra o makespan máximo entre as máquinas
    int maxMakespan = makespan[0];
    for (i = 1; i < numeroTasks; i++) {
        if (makespan[i] > maxMakespan) {
            maxMakespan = makespan[i];
        }
    }
    return maxMakespan;
}

void writesolucao(const char* nomeArquivo, int numeroTasks, Tarefa solucao[MAX_TASKS]) {
    FILE* arquivo = fopen(nomeArquivo, "w");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo %s.\n", nomeArquivo);
        return;
    }

    fprintf(arquivo, "Solução:\n");
    int i;
    for (i = 0; i < MAX_TASKS; i++) {
        if (solucao[i].task != -1) {
            fprintf(arquivo, "Máquina %d: ", solucao[i].task + 1);
            fprintf(arquivo, "%d", solucao[i].task);
            int j = i + 1;
            while (j < MAX_TASKS && solucao[j].task == solucao[i].task) {
                fprintf(arquivo, ", %d", solucao[j].task);
                j++;
            }
            fprintf(arquivo, "\n");
        }
    }
    int makespan = calculaMakespan(numeroTasks, solucao);
    fprintf(arquivo, "Makespan: %d\n", makespan);

    fclose(arquivo);
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
    int makespan = calculaMakespan(numeroTasks, solucao);
    printf("Makespan: %d\n", makespan);
}

void applyLocalSearch(int numeroTasks, Tarefa solucao[MAX_TASKS]) {
    int i, j;

    // Cria uma matriz auxiliar para indexar as tarefas por máquina
    Tarefa tarefasPorTask[MAX_TASKS][MAX_TASKS];
    int numTarefasPorTask[MAX_TASKS] = {0};

    // Preenche a matriz auxiliar com as tarefas
    for (i = 0; i < numeroTasks; i++) {
        int task = solucao[i].task;
        tarefasPorTask[task][numTarefasPorTask[task]] = solucao[i];
        numTarefasPorTask[task]++;
    }

    // Aplica a busca local
 for (i = 0; i < numeroTasks; i++) {
    for (j = 0; j < numeroTasks; j++) {
        if (j != i) {
            int k;
            for (k = 0; k < numTarefasPorTask[i]; k++) {
                int l;
                for (l = 0; l < numTarefasPorTask[j]; l++) {
                    // Realiza a troca das tarefas entre as máquinas i e j
                    Tarefa temp = tarefasPorTask[i][k];
                    tarefasPorTask[i][k] = tarefasPorTask[j][l];
                    tarefasPorTask[j][l] = temp;
                    applyLocalSearch(numeroTasks, tarefasPorTask); // Chamada recursiva para atualizar as trocas
                    temp = tarefasPorTask[i][k];
                    tarefasPorTask[i][k] = tarefasPorTask[j][l];
                    tarefasPorTask[j][l] = temp;
                }
            }
        }
    }
}


    // Atualiza o array de solução com as tarefas da matriz auxiliar
    int index = 0;
    for (i = 0; i < numeroTasks; i++) {
        for (j = 0; j < numTarefasPorTask[i]; j++) {
            solucao[index++] = tarefasPorTask[i][j];
        }
    }
}
*/

int main() {
    int numeroTasks;
    int tempoTask[MAX_TASKS];
    int precedence[MAX_TASKS][MAX_TASKS];

    readInstance("KILBRID.IN2", &numeroTasks, tempoTask, precedence);

<<<<<<< HEAD
    //Tarefa solucaoInicial[MAX_TASKS];
    //createsolucaoInicial(numeroTasks, tempoTask, solucaoInicial);

    //applyLocalSearch(numeroTasks, solucaoInicial);

    //int bestMakespan = calculaMakespan(numeroTasks, solucaoInicial);

    //writesolucao("output.txt", numeroTasks, solucaoInicial);

=======
    // Imprime os dados da instância para teste
    for (int i = 0; i < numeroTasks; i++) {
        for (int j = 0; j < numeroTasks; j++) {
            printf("| %d", precedence[i][j]);
        }
        printf("\n");
    }


    //Tarefa solucaoInicial[MAX_TASKS];
    //createsolucaoInicial(numeroTasks, tempoTask, solucaoInicial);

    //applyLocalSearch(numeroTasks, solucaoInicial);

    //int bestMakespan = calculaMakespan(numeroTasks, solucaoInicial);

    //writesolucao("output.txt", numeroTasks, solucaoInicial);

>>>>>>> d2e9570800099159273407ae8a74b10ebd57404f
    //printsolucao(numeroTasks, solucaoInicial);

    return 0;
}
