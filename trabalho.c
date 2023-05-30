
/*Trabalho 01 realizado para complemento de nota para Metodos de Otimização
UFES - Campus Alegre
Professor: Dayan de Castro Bissoli
Dupla: Talles Henrique 2020204803 - Alvaro Moret 
*/

#include <stdio.h>
#include <stdlib.h>

#define MAX_TAREFAS 100
#define MAX_MAQUINAS 5

typedef struct {
    int tempo;
    int indice;
} Tarefa;

void lerInstancia(FILE* arquivo, int* numeroTarefas, Tarefa tarefas[MAX_TAREFAS], int precedencias[MAX_TAREFAS][MAX_TAREFAS]) {
    fscanf(arquivo, "%d", numeroTarefas);

    for (int i = 0; i < *numeroTarefas; i++) {
        fscanf(arquivo, "%d", &tarefas[i].tempo);
        tarefas[i].indice = i + 1;
    }

    int i = 0, j = 0;
    while (fscanf(arquivo, "%d,%d", &i, &j) != EOF && i != -1 && j != -1) {
        precedencias[i - 1][j - 1] = 1;
    }
}

void criarSolucaoInicial(int numeroTarefas, Tarefa tarefas[MAX_TAREFAS], int precedencias[MAX_TAREFAS][MAX_TAREFAS], int maquinas[MAX_MAQUINAS][MAX_TAREFAS]) {
    int temposMaquinas[MAX_MAQUINAS] = {0};

    for (int i = 0; i < numeroTarefas; i++) {
        int indiceTarefa = tarefas[i].indice;
        int menorTempoMaquina = temposMaquinas[0];
        int maquinaAtual = 0;

        for (int j = 1; j < MAX_MAQUINAS; j++) {
            if (temposMaquinas[j] < menorTempoMaquina) {
                menorTempoMaquina = temposMaquinas[j];
                maquinaAtual = j;
            }
        }

        int posicaoTarefa = 0;
        while (maquinas[maquinaAtual][posicaoTarefa] != 0) {
            posicaoTarefa++;
        }

        maquinas[maquinaAtual][posicaoTarefa] = indiceTarefa;
        temposMaquinas[maquinaAtual] += tarefas[i].tempo;

        // Verificar as precedências e ajustar a máquina atual
        for (int j = 0; j < i; j++) {
            if (precedencias[indiceTarefa - 1][maquinas[maquinaAtual][j] - 1]) {
                maquinas[maquinaAtual][posicaoTarefa] = 0; // Coloca a tarefa no final da máquina atual
                break;
            }
        }
    }
}

int calcularMakespan(int numeroTarefas, Tarefa tarefas[MAX_TAREFAS], int maquinas[MAX_MAQUINAS][MAX_TAREFAS]) {
    int temposMaquinas[MAX_MAQUINAS] = {0};

    for (int i = 0; i < MAX_MAQUINAS; i++) {
        for (int j = 0; j < numeroTarefas; j++) {
            int indiceTarefa = maquinas[i][j];
            if (indiceTarefa > 0) {
                int tempoTarefa = tarefas[indiceTarefa - 1].tempo;
                temposMaquinas[i] += tempoTarefa;
            }
        }
    }

    int makespan = 0;
    for (int i = 0; i < MAX_MAQUINAS; i++) {
        if (temposMaquinas[i] > makespan) {
            makespan = temposMaquinas[i];
        }
    }

    int maiorMakespan = 0;
    int maquinaMaiorMakespan = 0;

    for (int i = 0; i < MAX_MAQUINAS; i++) {
        if (temposMaquinas[i] > maiorMakespan) {
            maiorMakespan = temposMaquinas[i];
            maquinaMaiorMakespan = i + 1;
        }
        printf("maquina :%d = %d\n", i+1, temposMaquinas[i]);
    }

    printf("Máquina com o maior makespan: %d\n", maquinaMaiorMakespan);


    return makespan;
}

void criarArquivoOutput(Tarefa tarefas[MAX_TAREFAS], int maquinas[MAX_MAQUINAS][MAX_TAREFAS], int makespan) {
    FILE* arquivo = fopen("output.txt", "w");
    if (arquivo == NULL) {
        printf("Erro ao criar o arquivo de saída.\n");
        return;
    }

    for (int i = 0; i < MAX_MAQUINAS; i++) {
        fprintf(arquivo, "Máquina %d: ", i + 1);

        int j = 0;
        while (maquinas[i][j] != 0) {
            int indiceTarefa = maquinas[i][j];
            fprintf(arquivo, "%d ", indiceTarefa);
            j++;
        }

        fprintf(arquivo, " (tarefas atendidas)\n");
    }

    fprintf(arquivo, "FO: %d\n", makespan);

    fclose(arquivo);

    printf("\nArquivo de saída criado com sucesso.\n");
}

void imprimirSolucao(Tarefa tarefas[MAX_TAREFAS], int maquinas[MAX_MAQUINAS][MAX_TAREFAS]) {
    for (int i = 0; i < MAX_MAQUINAS; i++) {
        printf("Máquina %d: ", i + 1);

        int j = 0;
        while (maquinas[i][j] != 0) {
            int indiceTarefa = maquinas[i][j];
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
    Tarefa tarefas[MAX_TAREFAS];
    int precedencias[MAX_TAREFAS][MAX_TAREFAS] = {0};
    int maquinas[MAX_MAQUINAS][MAX_TAREFAS] = {0};

    lerInstancia(arquivo, &numeroTarefas, tarefas, precedencias);
    fclose(arquivo);

    criarSolucaoInicial(numeroTarefas, tarefas, precedencias, maquinas);

    int makespan = calcularMakespan(numeroTarefas, tarefas, maquinas);

    criarArquivoOutput(tarefas, maquinas, makespan);
    imprimirSolucao(tarefas, maquinas);
    printf("FO: %d\n", makespan);

    return 0;
}