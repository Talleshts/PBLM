/* Trabalho 01 realizado para complemento de nota para Metodos de Otimização
UFES - Campus Alegre
Professor: Dayan de Castro Bissoli
Dupla: Talles Henrique 2020204803 - Alvaro Moret 
*/

#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int tempo;
    int indice;
} Tarefa;

void lerInstancia(FILE* arquivo, int* numeroTarefas, Tarefa** tarefas, int** precedencias) {
    fscanf(arquivo, "%d", numeroTarefas);

    *tarefas = (Tarefa*)malloc(*numeroTarefas * sizeof(Tarefa));
    *precedencias = (int*)calloc((*numeroTarefas) * (*numeroTarefas), sizeof(int));

    for (int i = 0; i < *numeroTarefas; i++) {
        fscanf(arquivo, "%d", &((*tarefas)[i].tempo));
        (*tarefas)[i].indice = i + 1;
    }

    int i = 0, j = 0;
    while (fscanf(arquivo, "%d,%d", &i, &j) != EOF && i != -1 && j != -1) {
        (*precedencias)[(i - 1) * (*numeroTarefas) + (j - 1)] = 1;
    }
}

void criarSolucaoInicial(int numeroTarefas, Tarefa* tarefas, int* precedencias, int numeroMaquinas, int** maquinas) {
    int* temposMaquinas = (int*)calloc(numeroMaquinas, sizeof(int));

    for (int i = 0; i < numeroTarefas; i++) {
        int indiceTarefa = tarefas[i].indice;
        int menorTempoMaquina = temposMaquinas[0];
        int maquinaAtual = 0;

        for (int j = 1; j < numeroMaquinas; j++) {
            if (temposMaquinas[j] < menorTempoMaquina) {
                menorTempoMaquina = temposMaquinas[j];
                maquinaAtual = j;
            }
        }

        int posicaoTarefa = 0;
        while ((*maquinas)[maquinaAtual * numeroTarefas + posicaoTarefa] != 0) {
            posicaoTarefa++;
        }

        (*maquinas)[maquinaAtual * numeroTarefas + posicaoTarefa] = indiceTarefa;
        temposMaquinas[maquinaAtual] += tarefas[i].tempo;

        // Verificar as precedências e ajustar a máquina atual
        for (int j = 0; j < i; j++) {
            if (precedencias[(indiceTarefa - 1) * numeroTarefas + ((*maquinas)[maquinaAtual * numeroTarefas + j] - 1)]) {
                (*maquinas)[maquinaAtual * numeroTarefas + posicaoTarefa] = 0; // Coloca a tarefa no final da máquina atual
                break;
            }
        }
    }

    free(temposMaquinas);
}

int calcularMakespan(int numeroTarefas, Tarefa* tarefas, int numeroMaquinas, int* maquinas) {
    int* temposMaquinas = (int*)calloc(numeroMaquinas, sizeof(int));

    for (int i = 0; i < numeroMaquinas; i++) {
        for (int j = 0; j < numeroTarefas; j++) {
            int indiceTarefa = maquinas[i * numeroTarefas + j];
            if (indiceTarefa > 0) {
                int tempoTarefa = tarefas[indiceTarefa - 1].tempo;
                temposMaquinas[i] += tempoTarefa;
            }
        }
    }

    int makespan = 0;
    for (int i = 0; i < numeroMaquinas; i++) {
        if (temposMaquinas[i] > makespan) {
            makespan = temposMaquinas[i];
        }
    }

    int maiorMakespan = 0;
    int maquinaMaiorMakespan = 0;

    for (int i = 0; i < numeroMaquinas; i++) {
        if (temposMaquinas[i] > maiorMakespan) {
            maiorMakespan = temposMaquinas[i];
            maquinaMaiorMakespan = i + 1;
        }
        printf("maquina :%d = %d\n", i+1, temposMaquinas[i]);
    }

    printf("Máquina com o maior makespan: %d\n", maquinaMaiorMakespan);

    free(temposMaquinas);

    return makespan;
}

void criarArquivoOutput(int numeroMaquinas, Tarefa* tarefas, int* maquinas, int makespan) {
    FILE* arquivo = fopen("output.txt", "w");
    if (arquivo == NULL) {
        printf("Erro ao criar o arquivo de saída.\n");
        return;
    }

    for (int i = 0; i < numeroMaquinas; i++) {
        fprintf(arquivo, "Máquina %d: ", i + 1);

        int j = 0;
        while (maquinas[i * numeroMaquinas + j] != 0) {
            int indiceTarefa = maquinas[i * numeroMaquinas + j];
            fprintf(arquivo, "%d ", indiceTarefa);
            j++;
        }

        fprintf(arquivo, " (tarefas atendidas)\n");
    }

    fprintf(arquivo, "FO: %d\n", makespan);

    fclose(arquivo);

    printf("Arquivo de saída criado com sucesso.\n");
}

void imprimirSolucao(int numeroMaquinas, Tarefa* tarefas, int* maquinas) {
    for (int i = 0; i < numeroMaquinas; i++) {
        printf("Máquina %d: ", i + 1);

        int j = 0;
        while (maquinas[i * numeroMaquinas + j] != 0) {
            int indiceTarefa = maquinas[i * numeroMaquinas + j];
            printf("%d ", indiceTarefa);
            j++;
        }

        printf(" (tarefas atendidas)");
        printf("\n");
    }
}

int main() {
    FILE* arquivo = fopen("KILBRID.IN2", "r");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return 1;
    }

    int numeroTarefas;
    Tarefa* tarefas;
    int* precedencias;
    int numeroMaquinas;

    lerInstancia(arquivo, &numeroTarefas, &tarefas, &precedencias);
    fclose(arquivo);

    printf("Digite o número de máquinas: ");
    scanf("%d", &numeroMaquinas);

    int* maquinas = (int*)calloc(numeroMaquinas * numeroTarefas, sizeof(int));

    criarSolucaoInicial(numeroTarefas, tarefas, precedencias, numeroMaquinas, &maquinas);

    int makespan = calcularMakespan(numeroTarefas, tarefas, numeroMaquinas, maquinas);

    criarArquivoOutput(numeroMaquinas, tarefas, maquinas, makespan);
    imprimirSolucao(numeroMaquinas, tarefas, maquinas);
    printf("FO: %d\n", makespan);

    free(tarefas);
    free(precedencias);
    free(maquinas);

    return 0;
}