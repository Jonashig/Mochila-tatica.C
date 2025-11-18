#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_ITENS 10

typedef struct {
    char nome[30];
    char tipo[20];
    int quantidade;
    int prioridade; // 1 (baixo) .. 5 (alto)
} Item;

// estado de ordenacao
// 0 = nao ordenado, 1 = por nome, 2 = por tipo, 3 = por prioridade
int ordenacao_status = 0;

// ---------- helpers de entrada ----------
void limparString(char *s) {
    size_t len = strlen(s);
    if (len > 0 && s[len-1] == '\n') s[len-1] = '\0';
}

void lerString(const char *prompt, char *dest, int maxlen) {
    char buffer[256];
    while (1) {
        printf("%s", prompt);
        if (!fgets(buffer, sizeof(buffer), stdin)) {
            // falha de IO, limpa e tenta de novo
            clearerr(stdin);
            continue;
        }
        limparString(buffer);
        if (strlen(buffer) == 0) {
            // aceita vazio? vamos pedir novamente
            printf("Entrada vazia. Tente novamente.\n");
            continue;
        }
        // truncar se maior que maxlen-1
        buffer[maxlen-1] = '\0';
        strncpy(dest, buffer, maxlen);
        dest[maxlen-1] = '\0';
        break;
    }
}

int lerInteiro(const char *prompt, int min, int max) {
    char buffer[256];
    int value;
    while (1) {
        printf("%s", prompt);
        if (!fgets(buffer, sizeof(buffer), stdin)) {
            clearerr(stdin);
            continue;
        }
        // tentar converter
        if (sscanf(buffer, "%d", &value) == 1) {
            if ((min == max && min == 0) || (value >= min && value <= max)) {
                return value;
            } else {
                if (min == max && min == 0)
                    ; // sem restricao
                else
                    printf("Valor fora do intervalo [%d - %d]. Tente novamente.\n", min, max);
            }
        } else {
            printf("Entrada invalida. Digite um numero inteiro.\n");
        }
    }
}

void pausar() {
    char tmp[64];
    printf("\nPressione ENTER para continuar...");
    fgets(tmp, sizeof(tmp), stdin); // espera um ENTER (uma leitura)
}

// ---------- exibição ----------
void exibirMenu(int contador) {
    printf("\nPLANO DE FUGA - CODIGO DA ILHA (NIVEL MESTRE)\n");
    printf("==============================================\n");
    printf("Itens na Mochila: %d/%d\n", contador, MAX_ITENS);
    printf("Status da Ordenacao: ");
    if (ordenacao_status == 0) printf("NAO ORDENADO\n\n");
    else if (ordenacao_status == 1) printf("ORDENADO POR NOME\n\n");
    else if (ordenacao_status == 2) printf("ORDENADO POR TIPO\n\n");
    else if (ordenacao_status == 3) printf("ORDENADO POR PRIORIDADE\n\n");

    printf("1. Adicionar Componente\n");
    printf("2. Descartar Componente\n");
    printf("3. Listar Componentes (Inventario)\n");
    printf("4. Organizar Mochila (Ordenar Componentes)\n");
    printf("5. Busca Binaria por Componente-Chave (por nome) [so se ordenado por nome]\n");
    printf("0. ATIVAR TORRE DE FUGA (Sair)\n");
    printf("----------------------------------------------\n");
    printf("Escolha uma opcao: ");
}

// imprimir cabeçalho da planilha
void imprimirCabecalhoTabela(int contador) {
    printf("\n--- ITENS NA MOCHILA (%d/%d) ---\n", contador, MAX_ITENS);
    printf("----------------------------------------------------------------\n");
    printf("%-14s | %-12s | %-10s | %-9s\n", "NOME", "TIPO", "QUANTIDADE", "PRIORIDADE");
    printf("----------------------------------------------------------------\n");
}

void imprimirLinhaItem(Item *it) {
    printf("%-14s | %-12s | %-10d | %-9d\n",
           it->nome, it->tipo, it->quantidade, it->prioridade);
}

// ---------- operações ----------
void inserirItem(Item mochila[], int *contador) {
    if (*contador >= MAX_ITENS) {
        printf("\nA mochila esta cheia! (%d/%d)\n", *contador, MAX_ITENS);
        pausar();
        return;
    }

    Item novo;
    lerString("\n--- Coletando Novo Componente ---\nNome: ", novo.nome, sizeof(novo.nome));
    lerString("Tipo (ex.: Estrutural, Eletronico, Energia): ", novo.tipo, sizeof(novo.tipo));
    novo.quantidade = lerInteiro("Quantidade: ", 1, 1000000);
    novo.prioridade = lerInteiro("Prioridade de Montagem (1-5): ", 1, 5);

    mochila[*contador] = novo;
    (*contador)++;
    ordenacao_status = 0; // ao adicionar, estado volta a "nao ordenado"
    printf("\nComponente inserido com sucesso!\n");
    pausar();
}

void removerItem(Item mochila[], int *contador) {
    if (*contador == 0) {
        printf("\nA mochila esta vazia! Nada para remover.\n");
        pausar();
        return;
    }
    char nomeBusca[30];
    lerString("\nDigite o nome do componente que deseja descartar: ", nomeBusca, sizeof(nomeBusca));
    int pos = -1;
    for (int i = 0; i < *contador; i++) {
        if (strcmp(mochila[i].nome, nomeBusca) == 0) {
            pos = i;
            break;
        }
    }
    if (pos == -1) {
        printf("\nNao encontrado %s na mochila.\n", nomeBusca);
        pausar();
        return;
    }
    for (int i = pos; i < *contador - 1; i++) mochila[i] = mochila[i+1];
    (*contador)--;
    printf("\nComponente removido com sucesso!\n");
    pausar();
}

void listarItens(Item mochila[], int contador) {
    if (contador == 0) {
        printf("\nA mochila esta vazia!\n");
        pausar();
        return;
    }
    imprimirCabecalhoTabela(contador);
    for (int i = 0; i < contador; i++) {
        imprimirLinhaItem(&mochila[i]);
    }
    printf("----------------------------------------------------------------\n");
    pausar();
}

// bubble sort por nome (alfabetico crescente), retorna numero de comparacoes
long ordenarPorNome(Item mochila[], int contador) {
    long comparacoes = 0;
    for (int i = 0; i < contador - 1; i++) {
        for (int j = 0; j < contador - 1 - i; j++) {
            comparacoes++;
            if (strcmp(mochila[j].nome, mochila[j+1].nome) > 0) {
                Item tmp = mochila[j];
                mochila[j] = mochila[j+1];
                mochila[j+1] = tmp;
            }
        }
    }
    return comparacoes;
}

// bubble sort por tipo (agrupa por tipo) - se tipos iguais mantém ordem relativa (estavel)
long ordenarPorTipo(Item mochila[], int contador) {
    long comparacoes = 0;
    for (int i = 0; i < contador - 1; i++) {
        for (int j = 0; j < contador - 1 - i; j++) {
            comparacoes++;
            if (strcmp(mochila[j].tipo, mochila[j+1].tipo) > 0) {
                Item tmp = mochila[j];
                mochila[j] = mochila[j+1];
                mochila[j+1] = tmp;
            }
        }
    }
    return comparacoes;
}

// bubble sort por prioridade (maior prioridade no topo) - decrescente
long ordenarPorPrioridade(Item mochila[], int contador) {
    long comparacoes = 0;
    for (int i = 0; i < contador - 1; i++) {
        for (int j = 0; j < contador - 1 - i; j++) {
            comparacoes++;
            if (mochila[j].prioridade < mochila[j+1].prioridade) {
                Item tmp = mochila[j];
                mochila[j] = mochila[j+1];
                mochila[j+1] = tmp;
            }
        }
    }
    return comparacoes;
}

void menuOrdenacao(Item mochila[], int contador) {
    if (contador == 0) {
        printf("\nA mochila esta vazia! Nada para ordenar.\n");
        pausar();
        return;
    }
    printf("\n--- ORGANIZAR MOCHILA ---\n");
    printf("Escolha o tipo de ordenacao:\n");
    printf("1 - Ordenar por NOME (A->Z)\n");
    printf("2 - Ordenar por TIPO (agrupa tipos)\n");
    printf("3 - Ordenar por PRIORIDADE (maior -> menor)\n");
    int opc = lerInteiro("Opcao: ", 1, 3);
    long cmp = 0;
    if (opc == 1) {
        cmp = ordenarPorNome(mochila, contador);
        ordenacao_status = 1;
        printf("\nOrdenacao por NOME concluida. Comparacoes efetuadas: %ld\n", cmp);
    } else if (opc == 2) {
        cmp = ordenarPorTipo(mochila, contador);
        ordenacao_status = 2;
        printf("\nOrdenacao por TIPO concluida. Comparacoes efetuadas: %ld\n", cmp);
    } else if (opc == 3) {
        cmp = ordenarPorPrioridade(mochila, contador);
        ordenacao_status = 3;
        printf("\nOrdenacao por PRIORIDADE concluida. Comparacoes efetuadas: %ld\n", cmp);
    }
    pausar();
}

// busca binaria por nome - so chamar se ordenado por nome
// retorna pos ou -1; conta comparacoes (passada por referencia)
int buscaBinariaPorNome(Item mochila[], int contador, const char *chave, long *comparacoes) {
    int low = 0, high = contador - 1;
    *comparacoes = 0;
    while (low <= high) {
        int mid = (low + high) / 2;
        (*comparacoes)++;
        int cmp = strcmp(mochila[mid].nome, chave);
        if (cmp == 0) return mid;
        else if (cmp < 0) low = mid + 1;
        else high = mid - 1;
    }
    return -1;
}

void buscarBinariaMenu(Item mochila[], int contador) {
    if (contador == 0) {
        printf("\nA mochila esta vazia! Nada para buscar.\n");
        pausar();
        return;
    }
    if (ordenacao_status != 1) {
        printf("\nBusca binaria disponivel somente quando a mochila esta ordenada por NOME (A->Z).\n");
        pausar();
        return;
    }
    char chave[30];
    lerString("\nDigite o NOME do componente a buscar (exato): ", chave, sizeof(chave));
    long comps = 0;
    int pos = buscaBinariaPorNome(mochila, contador, chave, &comps);
    if (pos != -1) {
        printf("\n--- Resultado da Busca (binaria) ---\n");
        printf("Comparacoes realizadas: %ld\n", comps);
        imprimirCabecalhoTabela(contador);
        imprimirLinhaItem(&mochila[pos]);
        printf("----------------------------------------------------------------\n");
    } else {
        printf("\nNao Encontrado %s na mochila. Comparacoes: %ld\n", chave, comps);
    }
    pausar();
}

// opcao de busca sequencial simples (mantida caso queira)
void buscarSequencialImprimir(Item mochila[], int contador) {
    if (contador == 0) {
        printf("\nA mochila esta vazia! Nada para buscar.\n");
        pausar();
        return;
    }
    char chave[30];
    lerString("\nDigite o NOME do componente a buscar (exato): ", chave, sizeof(chave));
    int pos = -1;
    long comparacoes = 0;
    for (int i = 0; i < contador; i++) {
        comparacoes++;
        if (strcmp(mochila[i].nome, chave) == 0) {
            pos = i;
            break;
        }
    }
    if (pos != -1) {
        printf("\nENCONTRADO %s %dx na mochila. (Comparacoes: %ld)\n", mochila[pos].nome, mochila[pos].quantidade, comparacoes);
        // imprimir formato tabelado apenas do item
        imprimirCabecalhoTabela(contador);
        imprimirLinhaItem(&mochila[pos]);
        printf("----------------------------------------------------------------\n");
    } else {
        printf("\nNao Encontrado %s na mochila. (Comparacoes: %ld)\n", chave, comparacoes);
    }
    pausar();
}

// ---------- main ----------
int main() {
    Item mochila[MAX_ITENS];
    int contador = 0;
    int opcao;

    do {
        exibirMenu(contador);
        opcao = lerInteiro("", 0, 9); // aceita 0..9, validacao dentro
        switch (opcao) {
            case 1: inserirItem(mochila, &contador); break;
            case 2: removerItem(mochila, &contador); break;
            case 3: listarItens(mochila, contador); break;
            case 4: menuOrdenacao(mochila, contador); break;
            case 5: buscarBinariaMenu(mochila, contador); break;
            case 0: printf("\nAtivando Torre de Fuga... Saindo.\n"); break;
            default:
                printf("\nOpcao invalida!\n");
                pausar();
        }
    } while (opcao != 0);

    return 0;
}