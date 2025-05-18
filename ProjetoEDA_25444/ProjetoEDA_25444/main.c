/**
 *  @file main.c
 *  @brief Programa principal para manipulacao de antenas, efeitos nefastos e grafos de antenas numa cidade.
 *  Este programa permite carregar antenas de um ficheiro, calcular efeitos nefastos,
 *  gerir antenas (adicionar/remover) e realizar operacoes sobre um grafo representando as antenas.
 *
 *  @author DavidPinheiro 
 *  @date 2025-05-17 

 *  @project ProjetoEDA_25444_FASE1_FASE2
 */
#include <stdio.h>
#include <stdlib.h>
#include "antena.h"
#include "grafo.h"


int main() {

    
    ListaLigada* lista = NULL;                         /* Ponteiro para a lista de antenas */
    ListaLigada* efeitosnefasto = NULL;                /* Ponteiro para a lista de efeitos nefastos */
    int opcao;                                         /* Opcao escolhida pelo utilizador no menu */
    int x, y;                                          /* Coordenadas da antena ou efeito */
    char frequencia;                                   /* Frequência da antena a ser adicionada */
    char matriz[12][12];                               /* Matriz 12x12 */

    const char* nomeFicheiro = "antenas.txt";          /* Nome do ficheiro que contem as antenas */

    GR* grafo = NULL;                                  /* Ponteiro para a estrutura do grafo de antenas. */

    char freqA, freqB;

    lista = CarregarFicheiro(nomeFicheiro);

    if (!CriarGrafo(lista, &grafo)) {
        return 0;
    }

    do {
        //lista = CarregarFicheiro(nomeFicheiro);
        efeitosnefasto = EfeitoNefasto(lista);
        AtualizarFicheiro(lista, efeitosnefasto, nomeFicheiro);
        
        // Exibir o menu
        Menu(&opcao);


        switch (opcao) {
        case 1:  // Listar as antenas
            system("cls");

            // Ler o conteudo do ficheiro para a matriz
            LerFicheiroParaMatriz(nomeFicheiro, matriz); 

            // Listar as antenas e os efeitos nefastos
            ListarAntenas(matriz, lista);  
            break;

        case 2:  // Visualizar o conteudo do ficheiro

            Ficheiro(nomeFicheiro);
            break;

        case 3:  // Adicionar uma antena
            system("cls");

            /*
            printf("------ Adicionar uma Antena ------\n");

            printf("Frequencia da Antena (Caractere): ");
            scanf_s(" %c", &frequencia, 1);

            printf("Coordenada X: ");
            scanf_s("%d", &x);

            printf("Coordenada Y: ");
            scanf_s("%d", &y);

            validarCoordenadas(x, y);

            */

            // Inserir a antena na lista
            InserirAntena(lista, 'B', 2, 2);
            InserirAntena(lista, 'C', 5, 5);
            InserirAntena(lista, 'A', 3, 3);
            InserirAntena(lista, 'A', 1, 1);
            InserirAntena(lista, 'D', 11, 11);


            // Recalcular o efeito nefasto e atualizar o arquivo
            efeitosnefasto = EfeitoNefasto(lista);
            AtualizarFicheiro(lista, efeitosnefasto, nomeFicheiro);
            
            free(grafo);
            CriarGrafo(lista, &grafo);

            break;

        case 4:  // Remover uma antena
            system("cls");

            /*
             printf("------ Remover uma Antena ------\n");

             printf("Coordenada X (0-11): ");
             scanf_s("%d", &x);

             printf("Coordenada Y (0-11): ");
             scanf_s("%d", &y);

             validarCoordenadas(x, y);

             */

            // Remover a antena da lista
            if (RemoverAntena(lista, 11, 11)) {

                // Recalcular o efeito nefasto e atualizar o arquivo
                efeitosnefasto = EfeitoNefasto(lista);
                AtualizarFicheiro(lista, efeitosnefasto, nomeFicheiro);

                free(grafo);
                CriarGrafo(lista, &grafo);
                
            }
            else {
                return 0;
            }
            break;

        case 5:  
            system("cls");

            /*
            printf("Introduza as coordenadas da antena para procura em profundidade\nCoordenada X: ");
            scanf_s("%d", &x);

            printf("Coordenada Y: ");
            scanf_s("%d", &y);

            Profundidade(grafo,x,y);
            */
            
            Profundidade(grafo,9, 9);

            break;

            
        case 6:
            system("cls");

            Largura(grafo);

            break;
        case 7: // Caminhos entre duas antenas            
            system("cls");

            // Ler o conteudo do ficheiro para a matriz
            LerFicheiroParaMatriz(nomeFicheiro, matriz);
            CaminhosEntreAntenas(grafo, nomeFicheiro);

            break;

        case 8: // Intersecoes de frequencias
            system("cls");

            ListarIntersecoes(grafo);

            break;
        case 9:// Libertar a memoria das listas 
            free(lista);
            free(efeitosnefasto);
            free(grafo);
            exit(0);

            break;
        default:
            return 0;
            break;
        }

    } while (opcao != 10);

    // Libertar a memoria das listas 
    free(lista);
    free(efeitosnefasto);
    free(grafo);
    return 0;
}
