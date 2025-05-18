/**
 *  @file antena.h
 *  @brief Definicao das estruturas de dados e prototipos das funcoes para manipulacao de antenas e efeitos nefastos.
 *  Este ficheiro contem as declaracoes para a estrutura `Antena` (representando uma antena)
 *  e `ListaAntenas` (uma lista ligada de antenas), bem como as funcoes para gerir estas estruturas.
 *
 *  @author DavidPinheiro 
 *  @date 2025-05-17
 *  @project ProjetoEDA_25444_FASE1_FASE2
 */

#ifndef ANTENA_H
#define ANTENA_H

#include <stdio.h>
#include <stdlib.h>


 /**
  *  @struct Antena
  *  @brief Estrutura que representa uma antena individual.
  *  Cada antena e definida pela sua frequencia de ressonancia (um caracter),
  *  as suas coordenadas (x, y) e um apontador para a proSxima antena na lista ligada.
  */
typedef struct Antena {
	char frequencia;		/* Frequencia da antena -> Caracter  */
	int x;					/* Coordenada X da antena */
	int y;					/* Coordenada Y da antena */
	struct Antena* prox;	/* Apontador para a proxima antena da lista ligada */
} Antena;

/**
 *  @struct ListaLigada
 *  @brief Estrutura que representa uma lista ligada de antenas.
 *  Contem um apontador para o primeiro elemento (h) da lista de antenas.
 *  Esta estrutura permite o armazenamento e manipulacao dinamica de multiplas antenas.
 */
typedef struct ListaLigada {
    Antena* h;				/* Apontador para o primeiro elemento da lista */
} ListaLigada;



int Menu(int* opcao);

ListaLigada* CarregarFicheiro(const char* nome_ficheiro);
ListaLigada* EfeitoNefasto(ListaLigada* lista);
ListaLigada* CriarLista();
int AtualizarFicheiro(ListaLigada* lista, ListaLigada* efeitos, const char* nome_ficheiro);

int LerFicheiroParaMatriz(const char* nomeFicheiro, char matriz[12][12]);
int ListarAntenas(char matriz[12][12], ListaLigada* lista);

int Ficheiro(char* nomeFicheiro);

int InserirAntena(ListaLigada* lista, char frequencia, int x, int y);

int RemoverAntena(ListaLigada* lista, int x, int y);

#endif  // !ANTENA_H


