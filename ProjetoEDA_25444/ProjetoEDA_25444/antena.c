/**
 *  @file antena.c
 *  @brief Implementacao das funcoes para manipulacao de antenas e calculo de efeitos nefastos.
 *  Este ficheiro contem a logica para criar, gerir listas de antenas, carregar dados de ficheiros,
 *  calcular efeitos nefastos e interagir com o utilizador atraves de um menu.
 *
 *  @author DavidPinheiro 
 *  @date 2025-05-17
 *  @project ProjetoEDA_25444_FASE1_FASE2
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include "antena.h"

#pragma region MENU
// Menu Opcoes 
int Menu(int* opcao)
{
    printf("\n------ Escolha uma opcao ------\n");
    printf("1. Listar as Antenas\n");
    printf("2. Ler Ficheiro das Antenas\n");
    printf("3. Adicionar uma Antena\n");
    printf("4. Remover uma Antena\n");
    printf("5. Procurar em Profundidade a partir de uma Antena\n");
    printf("6. Procurar em largura a partir de uma Antena\n");
    printf("7. Identificar todos os caminhos entre duas Anetnas\n");
    printf("8. Listar Intersecoes de Frequencias\n");
    printf("9. Sair\n");
    printf("R: ");
    scanf_s("%d", opcao);
    return 1;
}
#pragma endregion 

#pragma region START
ListaLigada* CarregarFicheiro(const char* nome_ficheiro) {
    FILE* ficheiro = fopen(nome_ficheiro, "r");
    if (!ficheiro) {
        return NULL;
    }

    ListaLigada* lista = CriarLista();
    char linha[100];
    int y = 0;
    while (fgets(linha, sizeof(linha), ficheiro)) {
        for (int x = 0; linha[x] != '\0' && linha[x] != '\n'; x++) {
            if (linha[x] != '.' && linha[x] != '#') {
                InserirAntena(lista, linha[x], x, y);
            }
        }
        y++;
    }

    fclose(ficheiro);
    return lista;
}

ListaLigada* EfeitoNefasto(ListaLigada* lista) {
    ListaLigada* locais_nefastos = CriarLista();
    Antena* a = lista->h;
    while (a) {
        Antena* b = a->prox;
        while (b) {
            if (a->frequencia == b->frequencia) {
                int dx = b->x - a->x;
                int dy = b->y - a->y;
                if (dx != 0 || dy != 0) {
                    InserirAntena(locais_nefastos, '#', a->x - dx, a->y - dy);
                    InserirAntena(locais_nefastos, '#', b->x + dx, b->y + dy);
                }
            }
            b = b->prox;
        }
        a = a->prox;
    }
    return locais_nefastos;
}


ListaLigada* CriarLista() {
    ListaLigada* lista = (ListaLigada*)malloc(sizeof(ListaLigada));
    lista->h = NULL;
    return lista;
}

int AtualizarFicheiro(ListaLigada* lista, ListaLigada* efeitos, const char* nome_ficheiro) {
    char matriz[12][12];
    for (int i = 0; i < 12; i++) {
        for (int j = 0; j < 12; j++) {
            matriz[i][j] = '.';
        }
    }

    // Atualiza com os efeitos nefastos
    Antena* atual = lista->h;
    while (atual) {
        matriz[atual->y][atual->x] = atual->frequencia;
        atual = atual->prox;
    }

    atual = efeitos->h;
    while (atual) {
        if (matriz[atual->y][atual->x] == '.') {
            matriz[atual->y][atual->x] = '#';
        }
        atual = atual->prox;
    }

    FILE* ficheiro = fopen(nome_ficheiro, "w");
    if (!ficheiro) {
        
        return 0;
    }

    for (int i = 0; i < 12; i++) {
        for (int j = 0; j < 12; j++) {
            fputc(matriz[i][j], ficheiro);
        }
        fputc('\n', ficheiro);
    }

    fclose(ficheiro);
    return 1;
}

#pragma endregion

#pragma region CASO 1
// Ler Ficheiro antenas.txt para Matriz[12][12]
int LerFicheiroParaMatriz(const char* nomeFicheiro, char matriz[12][12]) {
    FILE* file = fopen(nomeFicheiro, "r");
    if (!file)
    {
        return 0;
    }

    char linha[20]; // Buffer maior para evitar problemas

    for (int i = 0; i < 12; i++)
    {
        if (fgets(linha, sizeof(linha), file)) {
            for (int j = 0; j < 12; j++) {
                if (linha[j] != '\n' && linha[j] != '\0') {
                    matriz[i][j] = linha[j]; // Armazena cada caracter na matriz
                }
                else {
                    matriz[i][j] = ' '; // Caso seja um espaco vazio
                }
            }
        }
        else {
            for (int j = 0; j < 12; j++) {
                matriz[i][j] = ' '; // Preenche linhas vazias
            }
        }
    }

    fclose(file);
    return 1;
}
// Listar Antenas
int ListarAntenas(char matriz[12][12], ListaLigada* lista) {
    printf("\n------ Listar as Antenas ------\n");
    printf("+------------+----+----+\n");
    printf("| Frequencia |  X |  Y |\n");
    printf("+------------+----+----+\n");

    for (int i = 0; i < 12; i++) {
        for (int j = 0; j < 12; j++) {
            if (matriz[i][j] != '.' && matriz[i][j] != ' ' && matriz[i][j] != '#') {
                printf("|     %c      | %2d | %2d |\n", matriz[i][j], j, i);

            }
        }
    }
    printf("+------------+----+----+\n");
    printf("\n------ Locais com Efeito Nefasto ------\n");
    printf("+----+----+\n");
    printf("|  X |  Y |\n");
    printf("+----+----+\n");
    for (int i = 0; i < 12; i++) {
        for (int j = 0; j < 12; j++) {
            if (matriz[i][j] == '#') {
                printf("| %2d | %2d |\n", j, i);

            }
        }
    }
    return 1;
}
#pragma endregion

#pragma region CASO 2
// Imprime o ficheiro antenas.txt
int Ficheiro(char* nomeFicheiro) {

    FILE* file;
    char cart[256];
    errno_t err;

    err = fopen_s(&file, nomeFicheiro, "r");

    if (file == NULL)
    {
        return 0;
    }
    system("cls");
    printf("------ Ler Ficheiro das Antenas ------");
    printf("\n");
    while (fgets(cart, sizeof(cart), file) != NULL) {
        printf("%s", cart);
    }

    fclose(file);
    return 1;
}

#pragma endregion

#pragma region CASO 3
// Verificacao Coordenadas
int validarCoordenadas(int x, int y) {
    if (x < 0 || x >= 12 || y < 0 || y >= 12) {
        return 0;
    }
    return 1;
}
// Inserir Antena
int InserirAntena(ListaLigada* lista, char frequencia, int x, int y) {
    Antena* nova = (Antena*)malloc(sizeof(Antena));
    nova->frequencia = frequencia;
    nova->x = x;
    nova->y = y;
    nova->prox = lista->h;
    lista->h = nova;
    return 1;
}

#pragma endregion 

#pragma region CASO 4
// Remover Antena
int RemoverAntena(ListaLigada* lista, int x, int y) {
    Antena* atual = lista->h;
    Antena* anterior = NULL;
    while (atual) {
        if (atual->x == x && atual->y == y) {
            if (anterior) anterior->prox = atual->prox;
            else lista->h = atual->prox;
            free(atual);
            return 1;
        }
        
        anterior = atual;
        atual = atual->prox;
    }

    return 0;
}

#pragma endregion 
