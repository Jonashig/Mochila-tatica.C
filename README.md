Descrição breve

Sistema de mochila em linguagem C que simula um inventário de jogo de sobrevivência. Permite adicionar, remover, listar e buscar itens, além de ordenar por nome, tipo ou prioridade, exibindo o custo computacional das operações. Ideal para estudo de lógica, estruturas de dados e algoritmos de ordenação.


---

📦 Sobre o Projeto

Este projeto representa uma mochila de sobrevivência usada dentro de um jogo fictício chamado Código da Ilha. O jogador pode coletar itens, definir prioridades e interagir com sua mochila usando operações clássicas da computação.

O código evolui do “nível aventureiro” para o nível mestre, trazendo novas mecânicas como ordenações diferentes e busca binária.


---

🎒 Funcionalidades Principais

1. Adicionar item

O jogador insere:

Nome (até 30 caracteres)

Tipo (até 20 caracteres)

Quantidade (inteiro)

Prioridade (inteiro)


2. Remover item

Remove um item pelo nome, com deslocamento dos demais elementos.

3. Listar itens

Mostra os itens em formato de tabela organizada:

NOME            | TIPO          | QUANTIDADE | PRIORIDADE
----------------------------------------------------------
Kit Médico      | Consumo       | 2          | 5
Munição 9mm     | Armamento     | 50         | 3

4. Buscar item

Busca sequencial tradicional

Após lista estar em ordem alfabética → desbloqueia busca binária


5. Ordenações disponíveis

O jogador pode escolher entre três formas de organização:

a) Ordenar por Nome (A–Z)

Utiliza Bubble Sort

Exibe número total de comparações realizadas


b) Ordenar por Tipo

Agrupa itens de mesmo tipo

Conta e exibe comparações realizadas


c) Ordenar por Prioridade

Ordena do item de maior prioridade para o menor

Exibe número de comparações



---

🧠 Análise Computacional

Cada ordenação exibe o número de comparações realizadas, permitindo ao jogador entender o custo do algoritmo Bubble Sort em tempo real.

Exemplo:

Ordenação concluída!
Comparações realizadas: 45


---

🔍 Busca Binária (Modo Ultra Rápido)

Disponível apenas quando a mochila está ordenada alfabeticamente por nome.
A busca binária mostra:

Caminho percorrido

Comparações realizadas

Resultado (encontrado ou não)



---

🏗️ Estrutura do Código

Uso de struct para representar um item

Vetor fixo de até 10 itens

Funções separadas para modularidade

Entrada validada

Organização visual com tabelas simples



---

🎯 Objetivo Educacional

Projeto desenvolvido para disciplina de algoritmos e estrutura de dados, aplicando na prática:

Structs

Vetores

Ordenação (Bubble Sort)

Comparação de strings

Busca Sequencial

Busca Binária

Contagem de operações



---

📁 Requisitos

Compilador C (GCC recomendado)


gcc mochila.c -o mochila
./mochila
