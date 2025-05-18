/**
 *  @file grafo.h
 *  @brief Definição das estruturas de dados e prototipos das funcoes para manipulacao de grafos de antenas.
 *  Este ficheiro declara as estruturas `Adjacente` (para listas de adjacência), `Verrtice` (representando
 *  uma antena como um vertice) e `GR` (o grafo completo). Tambem inclui os prototipos
 *  das funcoes para criar, manipular e consultar o grafo.
 *
 *  @author DavidPinheiro 
 *  @date 2025-05-17
 *  @project ProjetoEDA_25444_FASE1_FASE2
 */

#ifndef GRAFO_H
#define GRAFO_H

#include <stdio.h>
#include <stdlib.h>
#include "antena.h"

 /**
  * @struct Adjacente
  * @brief Representa um no numa lista de adjacencia, indicando uma ligacao (aresta) a outro vertice.
  * Cada no contem o indice do vertice adjacente no array de vertices do grafo e um ponteiro
  * para o proximo no de adjacencia na lista.
  */
typedef struct Adjacente {
    int indice;               /* Indice do vertice adjacente */
    struct Adjacente* prox;    /* Apontador para o proximo adjacente na lista*/
} Adjacente;

/**
 * @struct Vertice
 * @brief Representa um vertice no grafo. Cada vertice corresponde a uma antena.
 * Contem um ponteiro para os dados da antena (Antena) e um ponteiro para a header
 * da lista de Adjcente (Adjacente).
 */
typedef struct Vertice {
    Antena* antena;            /* Ponteiro para a antena correspondente */
    Adjacente* adjacentes;     /* Lista de adjacentes */
} Vertice;


/**
 * @struct GR
 * @brief Representa o grafo completo das antenas.
 * ContEm um array de Vertice (os vertices do grafo) e o numero total de vertices.
 * As arestas sao implicitas atraves das listas de adjacencia de cada vertice.
 */
typedef struct GR {
    Vertice* vertices;         /* Array dinamico de todos os vertices */
    int numVertices;           /* Numero total de vertices (antenas) */
} GR;


int CriarGrafo(ListaLigada* lista, GR** grafo);
int MostrarGrafo(GR* grafo);

int Profundidade(GR* grafo, int x, int y);
int EncontrarIndicePorCoordenadas(GR* grafo, int x, int y);

int Largura(GR* grafo);

int CaminhosEntreAntenas(GR* grafo, char nomeFicheiro[]);
Antena ListarAntenasPorTipo(char tipo, char matriz[12][12]);
int ListarCaminhos(GR* grafo, int inicio, int fim);
int ListarCaminhosRec(GR* grafo, int atual, int destino, int* visitado, int* caminho, int pos);

int ListarIntersecoes(GR* grafo);

#endif  // !GRAFO_H