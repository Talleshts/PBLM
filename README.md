# PBLM
## Problema de balanceamento de linhas de montagem
O Problema de Balanceamento de Linhas de Montagem (PBLM) é um problema de otimização utilizado na indústria para equilibrar o fluxo de produção em uma linha de montagem, minimizando o tempo total de produção e o tempo ocioso dos trabalhadores e máquinas envolvidos no processo.

O objetivo do PBLM é distribuir as tarefas a serem executadas entre os postos de trabalho de forma que cada um deles tenha a mesma carga de trabalho, evitando gargalos e ociosidade. Isso envolve a definição da sequência de tarefas, tempos de execução, capacidade de cada posto e restrições de precedência entre as tarefas.

## Configuração do arquivo
- A primeira linha contém um único número inteiro n, que representa o número de tarefas na linha de montagem.
- As linhas 2 até n+1 contêm os custos das tarefas, representados por números inteiros positivos. O custo de uma tarefa é o tempo que ela leva para ser executada em um posto de trabalho.
- As linhas n+2 até o fim do arquivo contêm as relações de precedência direta entre as tarefas. Cada linha representa uma relação no formato "i,j", indicando que a tarefa i deve ser executada antes da tarefa j. Essas relações são importantes para garantir a consistência da ordem de execução das tarefas.
- A última linha do arquivo pode conter a marca final "-1,-1", indicando o fim da instância. Essa marca é opcional e nem sempre é utilizada.

Ao ler esse arquivo de texto em um programa PBLM, é possível criar uma representação interna da instância e aplicar técnicas de otimização para encontrar a melhor sequência de tarefas que equilibre a carga de trabalho na linha de montagem.
