/* Gabriel da Silva Marques Ferreira  3WA*/
/* Arthur Cury Sampaio  3WA*/

#include <stdio.h>
#include <stdlib.h>
#include "codifica.h"

void compacta(FILE *arqTexto, FILE *arqBin, struct compactadora *v) {
    char c;
    unsigned int Bucket = 0;
    int tamBucket = 0;

    while (fread(&c, 1, 1, arqTexto)) {  // Lê um caractere do arquivo de texto
        for (int i = 0; i < 32; i++) {   // Procura na tabela
            if (v[i].simbolo == c) {
                Bucket = (Bucket << v[i].tamanho) | v[i].codigo;  // Adiciona ao Bucket
                tamBucket += v[i].tamanho;

                while (tamBucket >= 8) {  // Se o Bucket tem pelo menos 8 bits, escreve no arquivo
                    char byte = (Bucket >> (tamBucket - 8)) & 0xFF;
                    fwrite(&byte, 1, 1, arqBin);
                    tamBucket -= 8;
                }
                break;
            }
        }
    }

    // Processar EOT
    Bucket = (Bucket << v[31].tamanho) | v[31].codigo;
    tamBucket += v[31].tamanho;
    while (tamBucket > 0) {  // Escreve o resto dos bits
        char byte = (Bucket >> (tamBucket - 8)) & 0xFF;
        if (tamBucket < 8) {
            byte &= (0xFF >> (8 - tamBucket));  // zera os bits mais significativos
        }
        fwrite(&byte, 1, 1, arqBin);
        tamBucket -= 8;
    }
}

void descompacta(FILE *arqBin, FILE *arqTexto, struct compactadora *v) {
    int byte, bit; // Armazenar byte lido e bit atual sendo processado
    unsigned int codAtual = 0; 
    int tamAtual = 0;
    int encontrado; // flag pra indicar se um código válido foi encontrado na tabela

    while ((byte = fgetc(arqBin)) != EOF) {
        for (int i = 7; i >= 0; i--) {
            bit = (byte >> i) & 1; // Leitura bit a bit
            codAtual = (codAtual << 1) | bit;
            tamAtual++;

            encontrado = 0;  
            for (int j = 0; j < 32; j++) {
                if (v[j].tamanho == tamAtual && v[j].codigo == codAtual) {
                    if (v[j].simbolo == v[31].simbolo) { // EOT
                        return;
                    }
                    fputc(v[j].simbolo, arqTexto);
                    codAtual = 0;
                    tamAtual = 0;
                    encontrado = 1; 
                    break;
                }
            }
            if (!encontrado && tamAtual == 32) { // Limite máximo de bits para evitar overflow se nenhum código válido for encontrado
                codAtual = 0;
                tamAtual = 0;
            }
        }
    }
}