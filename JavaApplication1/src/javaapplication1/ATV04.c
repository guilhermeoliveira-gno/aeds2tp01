#include <stdio.h>

void inverter(char str[], int tamanho, char resultado[]) { // funcao que recebe a palavra e devolve ela ao contrario

    for (int i = 0; i < tamanho; i++) {
        resultado[i] = str[tamanho - 1 - i]; // vai pegando as letras do fim pro inicio
    }

    resultado[tamanho] = '\0'; // marca o fim da palavra
}

int main() { //t

    char entrada[1000];
    char invertida[1000];

    while (fgets(entrada, sizeof(entrada), stdin) != NULL) { // fica lendo as palavras ate acabar a entrada

        int tamanho = 0;

        while (entrada[tamanho] != '\0' && entrada[tamanho] != '\n') {
            tamanho++; // conta quantas letras tem, sem contar o enter do final
        }

        inverter(entrada, tamanho, invertida); // chama a funcao pra inverter e salva o resultado

        printf("%s\n", invertida); // mostra a palavra invertida na tela
    }

    return 0;
}
