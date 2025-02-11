#include <stdio.h>
#include <stdlib.h>
#include "codifica.h"

void hex_dump(FILE *fp) {
    unsigned char buffer[16];  // Buffer para armazenar os dados lidos
    size_t bytes_lidos;
    int i;

    if (fp == NULL) {
        fprintf(stderr, "Erro: ponteiro de arquivo inválido.\n");
        return;
    }

    // Lê o arquivo em blocos de 16 bytes
    while ((bytes_lidos = fread(buffer, 1, 16, fp)) > 0) {
        // Mostra cada byte em hexadecimal
        for (i = 0; i < bytes_lidos; i++) {
            printf("%02x ", buffer[i]);
        }
        printf("\n");
    }
}

typedef struct node {
    char simbolo;
    struct node* left;
    struct node* right;
} Node;

Node* createNode(char simbolo) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->simbolo = simbolo;
    newNode->left = newNode->right = NULL;
    return newNode;
}

void insert(Node *root, unsigned int codigo, int tamanho, char simbolo) {
    Node *current = root;
    for (int i = tamanho - 1; i >= 0; i--) {
        int bit = (codigo >> i) & 1;
        if (bit == 0) {
            if (!current->left) {
                current->left = createNode('\0');
            }
            current = current->left;
        } else {
            if (!current->right) {
                current->right = createNode('\0');
            }
            current = current->right;
        }
    }
    current->simbolo = simbolo;
}

void freeTree(Node *root) {
    if (root) {
        freeTree(root->left);
        freeTree(root->right);
        free(root);
    }
}

int main() {
    FILE *arqTexto = fopen("teste1.txt", "r");
    FILE *arqBin = fopen("teste1.bin", "wb");


    struct compactadora test1[32] = {
        {'a',60,6},
        {'b',61,6},
        {'c',62,6},
        {'d',63,6},
        {'e',5,5},
        {'f',6,5},
        {'g',7,5},
        {'h',8,5},
        {'i',9,5},
        {'j',10,5},
        {'k',11,5},
        {'l',12,5},
        {'m',13,5},
        {'n',14,5},
        {'o',15,5},
        {'p',16,5},
        {'r',17,5},
        {'s',18,5},
        {'+',19,5},
        {'t',20,5},
        {'u',21,5},
        {'v',22,5},
        {'w',23,5},
        {'x',24,5},
        {'y',25,5},
        {'z',26,5},
        {'.',3,5},
        {';',27,5},
        {'=',2,5},
        {'\n',14,4},
        {0,0,0},
        {4,0,5}

    };
    struct compactadora tabela[32] = {
        {'K', 1, 12},
        {'W', 2, 12},
        {'Y', 3, 12},
        {'Z', 2, 11},
        {'J', 3, 11},
        {'X', 1, 9},
        {'\n', 1, 8},
        {'H', 1, 7},
        {'F', 36, 7},
        {'G', 37, 7},
        {'B', 1, 6},
        {'Q', 16, 6},
        {',', 17, 6},
        {'V', 19, 6},
        {'.', 49, 6},
        {'P', 48, 6},
        {'D', 1, 5},
        {'C', 17, 5},
        {'O', 16, 5},
        {'M', 25, 5},
        {'R', 1, 4},
        {'L', 7, 4},
        {'N', 6, 4},
        {'A', 5, 4},
        {'T', 9, 4},
        {'S', 14, 4},
        {'U', 15, 4},
        {'I', 13, 4},
        {'E', 1, 3},
        {' ', 5, 3},
        {0, 0, 0},  
        {4, 0, 12}  
    };

    if (!arqTexto || !arqBin) {
        perror("Erro ao abrir os arquivos");
        return EXIT_FAILURE;
    }
    compacta(arqTexto, arqBin, tabela);


    fclose(arqTexto);
    fclose(arqBin);

    arqBin = fopen("teste1.bin","rb");

    hex_dump(arqBin);
    fclose(arqBin);

    arqBin = fopen("teste1.bin", "rb");
    FILE *arqTextoOut = fopen("teste1_out.txt", "w");

    if (!arqBin || !arqTextoOut) {
        perror("Erro ao abrir os arquivos");
        return EXIT_FAILURE;
    }

    descompacta(arqBin, arqTextoOut, tabela);

    fclose(arqBin);
    fclose(arqTextoOut);

    FILE *arqBin2 = fopen("teste2.bin", "rb");
    FILE *arqTextoOut2 = fopen("teste2_out.txt","w");

    if (!arqBin2 || !arqTextoOut2) {
        perror("Erro ao abrir os arquivos");
        return EXIT_FAILURE;
    }

    descompacta(arqBin2, arqTextoOut2, tabela);

    fclose(arqBin2);
    fclose(arqTextoOut2);

    return EXIT_SUCCESS;
}