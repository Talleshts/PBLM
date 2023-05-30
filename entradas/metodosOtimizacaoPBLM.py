class Tarefa:
    def __init__(self, tempo, indice):
        self.tempo = tempo
        self.indice = indice

def lerInstancia(arquivo):
    numeroTarefas = int(arquivo.readline().strip())

    tarefas = []
    precedencias = [[] for _ in range(numeroTarefas)]

    for i in range(numeroTarefas):
        tempo = int(arquivo.readline().strip())
        tarefa = Tarefa(tempo, i + 1)
        tarefas.append(tarefa)

    for _ in range(numeroTarefas):
        linha = arquivo.readline().strip()
        if linha:
            i, j = map(int, linha.split(","))
            precedencias[j - 1].append(i)

    return numeroTarefas, tarefas, precedencias

def criarSolucaoInicial(numeroTarefas, tarefas, precedencias, numMaquinas):
    maquinas = [[] for _ in range(numMaquinas)]
    temposMaquinas = [0] * numMaquinas

    for i in range(numeroTarefas):
        indiceTarefa = tarefas[i].indice
        maquinaAtual = min(range(numMaquinas), key=temposMaquinas.__getitem__)

        posicaoTarefa = len(maquinas[maquinaAtual])
        maquinas[maquinaAtual].append(indiceTarefa)
        temposMaquinas[maquinaAtual] += tarefas[i].tempo

        for j in range(i):
            if any(tarefa in maquinas[maquinaAtual] for tarefa in precedencias[indiceTarefa - 1]):
                maquinas[maquinaAtual][posicaoTarefa] = 0
                break

    return maquinas

def calcularMakespan(numeroTarefas, tarefas, maquinas):
    numMaquinas = len(maquinas)
    temposMaquinas = [sum(tarefas[maquinas[i][j] - 1].tempo for j in range(len(maquinas[i])) if maquinas[i][j] > 0) for i in range(numMaquinas)]
    makespan = max(temposMaquinas)
    maquinaMaiorMakespan = temposMaquinas.index(makespan) + 1

    print("Máquina com o maior makespan:", maquinaMaiorMakespan)

    return makespan

def criarArquivoOutput(tarefas, maquinas, makespan):
    with open("output.txt", "w") as arquivo:
        for i in range(len(maquinas)):
            arquivo.write(f"Máquina {i + 1}: ")
            arquivo.write(" ".join(str(maquinas[i][j]) for j in range(len(maquinas[i])) if maquinas[i][j] != 0))
            arquivo.write(" (tarefas atendidas)\n")

        arquivo.write(f"FO: {makespan}\n")

    print("Arquivo de saída criado com sucesso.")

def imprimirSolucao(tarefas, maquinas):
    for i in range(len(maquinas)):
        print(f"Máquina {i + 1}: ", end="")
        print(" ".join(str(maquinas[i][j]) for j in range(len(maquinas[i])) if maquinas[i][j] != 0), end="")
        print(" (tarefas atendidas)")

def main():
    numMaquinas = int(input("Digite a quantidade de máquinas: "))
    
    with open("instancia.txt", "r") as arquivo:
        numeroTarefas, tarefas, precedencias = lerInstancia(arquivo)

    maquinas = criarSolucaoInicial(numeroTarefas, tarefas, precedencias, numMaquinas)

    makespan = calcularMakespan(numeroTarefas, tarefas, maquinas)

    criarArquivoOutput(tarefas, maquinas, makespan)
    imprimirSolucao(tarefas, maquinas)
    print(f"FO: {makespan}")

if __name__ == "__main__":
    main()
