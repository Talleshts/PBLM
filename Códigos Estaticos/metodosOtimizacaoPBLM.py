MAX_TAREFAS = 100
MAX_MAQUINAS = 4

class Tarefa:
    def __init__(self, tempo, indice):
        self.tempo = tempo
        self.indice = indice

def lerInstancia(arquivo):
    numeroTarefas = int(arquivo.readline().strip())

    tarefas = []
    precedencias = [[0] * MAX_TAREFAS for _ in range(MAX_TAREFAS)]

    for i in range(numeroTarefas):
        tempo = int(arquivo.readline().strip())
        tarefa = Tarefa(tempo, i + 1)
        tarefas.append(tarefa)

    while True:
        line = arquivo.readline().strip()
        if not line:
            break

        i, j = map(int, line.split(","))
        precedencias[i - 1][j - 1] = 1

    return numeroTarefas, tarefas, precedencias

def criarSolucaoInicial(numeroTarefas, tarefas, precedencias):
    maquinas = [[0] * MAX_TAREFAS for _ in range(MAX_MAQUINAS)]
    temposMaquinas = [0] * MAX_MAQUINAS

    for i in range(numeroTarefas):
        indiceTarefa = tarefas[i].indice
        maquinaAtual = min(range(MAX_MAQUINAS), key=temposMaquinas.__getitem__)

        posicaoTarefa = maquinas[maquinaAtual].index(0)
        maquinas[maquinaAtual][posicaoTarefa] = indiceTarefa
        temposMaquinas[maquinaAtual] += tarefas[i].tempo

        for j in range(i):
            if precedencias[indiceTarefa - 1][maquinas[maquinaAtual][j] - 1]:
                maquinas[maquinaAtual][posicaoTarefa] = 0
                break

    return maquinas

def calcularMakespan(numeroTarefas, tarefas, maquinas):
    temposMaquinas = [sum(tarefas[maquinas[i][j] - 1].tempo for j in range(numeroTarefas) if maquinas[i][j] > 0) for i in range(MAX_MAQUINAS)]
    makespan = max(temposMaquinas)
    maquinaMaiorMakespan = temposMaquinas.index(makespan) + 1

    print("Máquina com o maior makespan:", maquinaMaiorMakespan)

    return makespan

def criarArquivoOutput(tarefas, maquinas, makespan):
    with open("output.txt", "w") as arquivo:
        for i in range(MAX_MAQUINAS):
            arquivo.write(f"Máquina {i + 1}: ")
            arquivo.write(" ".join(str(maquinas[i][j]) for j in range(MAX_TAREFAS) if maquinas[i][j] != 0))
            arquivo.write(" (tarefas atendidas)\n")

        arquivo.write(f"FO: {makespan}\n")

    print("Arquivo de saída criado com sucesso.")

def imprimirSolucao(tarefas, maquinas):
    for i in range(MAX_MAQUINAS):
        print(f"Máquina {i + 1}:", end=" ")
        print(" ".join(str(maquinas[i][j]) for j in range(MAX_TAREFAS) if maquinas[i][j] != 0), end="")
        print(" (tarefas atendidas)")

def main():
    with open("KILBRID.IN2", "r") as arquivo:
        numeroTarefas, tarefas, precedencias = lerInstancia(arquivo)

    maquinas = criarSolucaoInicial(numeroTarefas, tarefas, precedencias)

    makespan = calcularMakespan(numeroTarefas, tarefas, maquinas)

    criarArquivoOutput(tarefas, maquinas, makespan)
    imprimirSolucao(tarefas, maquinas)
    print(f"FO: {makespan}")

if __name__ == "__main__":
    main()