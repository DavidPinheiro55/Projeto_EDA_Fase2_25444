/**
 *  @file grafo.c
 *  @brief Implementacao das funcoes para manipulacao de grafos de antenas.
 *  Este ficheiro contem a logica para criar grafos a partir de listas de antenas,
 *  realizar procuras (Profundidade e Largura), encontrar caminhos e outras operacoes sobre grafos.
 *
 *  @author DavidPinheiro 
 *  @date 2025-05-17
 *  @project ProjetoEDA_25444_FASE1_FASE2
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "grafo.h"

#pragma region GERAL
int CriarGrafo(ListaLigada* lista, GR** grafo) {
    Antena* atual = lista->h;
    int contador = 0;

    while (atual) {
        contador++;
        atual = atual->prox;
    }

    *grafo = (GR*)malloc(sizeof(GR));
    if (!*grafo) return 0;

    (*grafo)->vertices = (Vertice*)malloc(sizeof(Vertice) * contador);
    if (!(*grafo)->vertices) return 0;

    (*grafo)->numVertices = contador;
    atual = lista->h;

    for (int i = 0; i < contador; i++) {
        (*grafo)->vertices[i].antena = atual;
        (*grafo)->vertices[i].adjacentes = NULL;
        atual = atual->prox;
    }

    for (int i = 0; i < contador; i++) {
        for (int j = 0; j < contador; j++) {
            if (i != j &&
                (*grafo)->vertices[i].antena->frequencia == (*grafo)->vertices[j].antena->frequencia) {
                Adjacente* novo = (Adjacente*)malloc(sizeof(Adjacente));
                if (!novo) return 0;
                novo->indice = j;
                novo->prox = (*grafo)->vertices[i].adjacentes;
                (*grafo)->vertices[i].adjacentes = novo;
            }
        }
    }

    return 1;
}

int MostrarGrafo(GR* grafo) {
    if (!grafo) return 0;

    printf("\n------ Grafo de Antenas ------\n");
    for (int i = 0; i < grafo->numVertices; i++) {
        printf("Antena %c (%d,%d): ",
            grafo->vertices[i].antena->frequencia,
            grafo->vertices[i].antena->x,
            grafo->vertices[i].antena->y);

        Adjacente* adj = grafo->vertices[i].adjacentes;
        while (adj) {
            printf("-> %c(%d,%d) ",
                grafo->vertices[adj->indice].antena->frequencia,
                grafo->vertices[adj->indice].antena->x,
                grafo->vertices[adj->indice].antena->y);
            adj = adj->prox;
        }
        printf("\n");
    }
    return 1;
}
#pragma endregion

#pragma region CASO 5
int Profundidade(GR* grafo, int x, int y) {
    if (!grafo) return 0;

    int indiceInicio = EncontrarIndicePorCoordenadas(grafo, x, y);
    if (indiceInicio == -1) {
        return 0;
    }

    int* list = calloc(grafo->numVertices, sizeof(int));
    int* pilha = malloc(sizeof(int) * grafo->numVertices);
    int topo = 0;
    pilha[topo++] = indiceInicio;

    printf("\n------ Procurar Profundidade a partir de (%d,%d) ------\n", x, y);

    while (topo > 0) {
        int atual = pilha[--topo];
        if (!list[atual]) {
            list[atual] = 1;
            printf("Encontrado: %c(%d,%d)\n",
                grafo->vertices[atual].antena->frequencia,
                grafo->vertices[atual].antena->x,
                grafo->vertices[atual].antena->y);

            Adjacente* adj = grafo->vertices[atual].adjacentes;
            while (adj) {
                if (!list[adj->indice]) {
                    pilha[topo++] = adj->indice;
                }
                adj = adj->prox;
            }
        }
    }

    free(pilha);
    free(list);
    return 1;
}

int EncontrarIndicePorCoordenadas(GR* grafo, int x, int y) {
    for (int i = 0; i < grafo->numVertices; i++) {
        if (grafo->vertices[i].antena->x == x && grafo->vertices[i].antena->y == y) {
            return i;
        }
    }
    return -1;
}
#pragma endregion

#pragma region CASO 6
int Largura(GR* grafo) {
    if (!grafo) return 0;

    int x, y;
    printf("Introduza as coordenadas da antena para procura em largura\nCoordenada X: ");
    scanf_s("%d", &x);

    printf("Coordenada Y: ");
    scanf_s("%d", &y);

    int indiceInicio = EncontrarIndicePorCoordenadas(grafo, x, y);
    if (indiceInicio == -1) {
        return 0;
    }

    int* list = calloc(grafo->numVertices, sizeof(int));
    int* pilha = malloc(sizeof(int) * grafo->numVertices);
    int inicio = 0, fim = 0;

    pilha[fim++] = indiceInicio;
    list[indiceInicio] = 1;

    printf("\n------ Procurar Largura a partir de (%d,%d) ------\n", x, y);

    while (inicio < fim) {
        int atual = pilha[inicio++];
        printf("Encontrado: %c(%d,%d)\n",
            grafo->vertices[atual].antena->frequencia,
            grafo->vertices[atual].antena->x,
            grafo->vertices[atual].antena->y);

        Adjacente* adj = grafo->vertices[atual].adjacentes;
        while (adj) {
            if (!list[adj->indice]) {
                pilha[fim++] = adj->indice;
                list[adj->indice] = 1;
            }
            adj = adj->prox;
        }
    }

    free(pilha);
    free(list);
    return 1;
}
#pragma endregion

#pragma region CASO 7
int CaminhosEntreAntenas(GR* grafo, char nomeFicheiro[]) {
    char ant, ant2;
    Antena C1, C2;
    char matriz[12][12]; // assume tamanho definido globalmente ou como macro

    LerFicheiroParaMatriz(nomeFicheiro, matriz);

    printf("Escolha a Primeira Antena: ");
    scanf_s(" %c", &ant);
    C1 = ListarAntenasPorTipo(ant, matriz);

    printf("Escolha a Segunda Antena: ");
    scanf_s(" %c", &ant2);
    C2 = ListarAntenasPorTipo(ant2, matriz);

    // Impede caminhos entre antenas de frequências diferentes
    if (ant != ant2) {
        return 0;
    }

    // Obtem os índices das antenas no grafo
    int indiceInicio = EncontrarIndicePorCoordenadas(grafo, C1.x, C1.y);
    int indiceFim = EncontrarIndicePorCoordenadas(grafo, C2.x, C2.y);

    if (indiceInicio != -1 && indiceFim != -1) {
        ListarCaminhos(grafo, indiceInicio, indiceFim);
    }
    else {
        return 0;
    }

    return 1;
}

Antena ListarAntenasPorTipo(char tipo, char matriz[12][12]) {
    Antena opcoes[20]; // no máximo 20 antenas do mesmo tipo
    int escolha;
    int count = 0;

    printf("Antena(s) do tipo %c:\n", tipo);
    for (int i = 0; i < 12; i++) {
        for (int j = 0; j < 12; j++) {
            if (matriz[i][j] == tipo) {
                count++;
                printf("%d - %c(%2d, %2d)\n", count, tipo, j, i);
                opcoes[count - 1].x = j;
                opcoes[count - 1].y = i;
            }
        }
    }

    if (count == 0) {
        return (Antena) { -1, -1 }; // invalido
    }

    printf("Escolha: ");
    scanf_s("%d", &escolha);
    while (escolha < 1 || escolha > count) {
        printf("Escolha: ");
        scanf_s("%d", &escolha);
    }

    return opcoes[escolha - 1];
}

int ListarCaminhos(GR* grafo, int inicio, int fim) {
    int* list = (int*)calloc(grafo->numVertices, sizeof(int));
    int* caminho = (int*)malloc(grafo->numVertices * sizeof(int));

    printf("\n------ Caminhos possiveis ------\n");
    ListarCaminhosRec(grafo, inicio, fim, list, caminho, 0);

    free(list);
    free(caminho);
    return 1;
}

int ListarCaminhosRec(GR* grafo, int atual, int destino, int* list, int* caminho, int pos) {
    list[atual] = 1;
    caminho[pos] = atual;
    pos++;

    if (atual == destino) {
        printf("Caminho encontrado: ");
        for (int i = 0; i < pos; i++) {
            Antena* a = grafo->vertices[caminho[i]].antena;
            printf("%c(%d,%d)", a->frequencia, a->x, a->y);
            if (i < pos - 1) printf(" -> ");
        }
        printf("\n");
    }
    else {
        Adjacente* adj = grafo->vertices[atual].adjacentes;
        while (adj) {
            if (!list[adj->indice]) {
                ListarCaminhosRec(grafo, adj->indice, destino, list, caminho, pos);
            }
            adj = adj->prox;
        }
    }

    list[atual] = 0; // backtrack
    return 1;
}
#pragma endregion

#pragma region CASO 8
int ListarIntersecoes(GR* grafo) {
    if (!grafo) return 0;

    char freqA, freqB;
    printf("Frequencia A: ");
    scanf_s(" %c", &freqA, 1);

    printf("Frequencia B: ");
    scanf_s(" %c", &freqB, 1);

    printf("\n------ Intersecoes entre frequencias %c e %c ------\n", freqA, freqB);

    for (int i = 0; i < grafo->numVertices; i++) {
        if (grafo->vertices[i].antena->frequencia == freqA) {
            for (int j = 0; j < grafo->numVertices; j++) {
                if (grafo->vertices[j].antena->frequencia == freqB) {
                    printf("Intersecao: %c(%d,%d) <-> %c(%d,%d)\n",
                        grafo->vertices[i].antena->frequencia,
                        grafo->vertices[i].antena->x,
                        grafo->vertices[i].antena->y,
                        grafo->vertices[j].antena->frequencia,
                        grafo->vertices[j].antena->x,
                        grafo->vertices[j].antena->y);
                    
                }
            }
        }
    }

    return 1;
}
#pragma endregion
