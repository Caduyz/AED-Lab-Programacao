# Sistema Acadêmico Inteligente

Sistema acadêmico desenvolvido em linguagem C para a disciplina de Laboratório de Programação.

O projeto tem como objetivo demonstrar o uso de estruturas fundamentais da linguagem C, incluindo:

* Entrada e saída de dados
* Estruturas condicionais
* Estruturas de repetição
* Vetores
* Matrizes
* Strings
* Modularização com funções

## Funcionalidades

* Cadastro de alunos
* Busca de alunos por nome ou matrícula
* Lançamento de notas
* Cálculo automático de médias
* Classificação:

  * Aprovado
  * Recuperação
  * Reprovado
* Relatório completo da turma
* Estatísticas gerais:

  * Média geral
  * Maior média
  * Menor média
  * Quantidade de aprovados/reprovados
* Ordenação dos alunos por média utilizando Bubble Sort
* Interface de terminal com cores ANSI
* Tratamento de entradas inválidas

## Tecnologias Utilizadas

* Linguagem C
* GCC
* Terminal ANSI

## Compilação

### Linux / macOS

```bash
gcc main.c -o sistema
./sistema
```

### Windows (MinGW)

```bash
gcc main.c -o sistema.exe
sistema.exe
```

## Estrutura do Projeto

O sistema foi desenvolvido utilizando programação procedural e modularização por funções, mantendo toda a implementação em um único arquivo `.c`.

## Observações

Este projeto foi desenvolvido para fins acadêmicos como atividade prática (**A**tividade **E**xterna da **D**isciplina) da disciplina de Laboratório de Programação.
